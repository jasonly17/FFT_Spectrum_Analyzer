#include "AudioEngine.h"

#include <QtCore/QBitArray>
#include <QtCore/QDebug>

#include "Preferences.h"

AudioEngine::AudioEngine(QObject *parent)
	: QThread(parent)
	, m_audioInput(nullptr), m_ioDevice(nullptr)
	, m_readySegment(-1)
	, m_segmentRead(false)
	, m_state(ProcessingState::Stopped)
{
	m_format.setByteOrder(QAudioFormat::LittleEndian);
	m_format.setSampleType(QAudioFormat::SignedInt);
	m_format.setCodec("audio/pcm");
	updateAudioFormat();

	updateBuffer();

	QThread::start();
}

AudioEngine::~AudioEngine()
{
	QMutexLocker locker(&m_mutex);
	if (m_audioInput != nullptr) {
		disconnect(m_ioDevice, SIGNAL(readyRead()), this, SLOT(audioDataReady()));
		m_audioInput->stop();
		m_audioInput->deleteLater();
	}

	QThread::wait();
}

void AudioEngine::start()
{
	QMutexLocker locker(&m_mutex);
	if (m_state == ProcessingState::Aborted) return;

	if (m_audioInput != nullptr && m_state != ProcessingState::Stopped) {
		m_audioInput->stop();
		m_audioInput->deleteLater();
	}

	const int audioDeviceInfoIndex = Preferences::getInstance().getAudioInputDevice();
	QAudioDeviceInfo audioDeviceInfo = QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(audioDeviceInfoIndex);

	m_audioInput = new QAudioInput(audioDeviceInfo, m_format, this);
	m_ioDevice = m_audioInput->start();
	connect(m_ioDevice, SIGNAL(readyRead()), this, SLOT(audioDataReady()));

	m_state = ProcessingState::Running;
	m_waitCond.wakeOne();
}

void AudioEngine::suspend()
{
	m_mutex.lock();

	if (m_audioInput != nullptr && m_audioInput->state() == QAudio::ActiveState) {
		m_audioInput->suspend();
	}

	m_state = ProcessingState::Suspended;
	m_waitCond.wakeOne();
	m_mutex.unlock();
}

void AudioEngine::resume()
{
	m_mutex.lock();

	if (m_audioInput != nullptr && m_audioInput->state() == QAudio::SuspendedState) {
		m_audioInput->resume();
	}

	m_state = ProcessingState::Running;
	m_waitCond.wakeOne();
	m_mutex.unlock();
}

void AudioEngine::stop()
{
	m_mutex.lock();

	if (m_audioInput != nullptr) {
		m_audioInput->stop();
		m_audioInput->deleteLater();
		m_audioInput = nullptr;
		m_ioDevice = nullptr;
	}

	m_state = ProcessingState::Stopped;
	m_waitCond.wakeOne();
	m_mutex.unlock();
}

void AudioEngine::terminate()
{
	m_mutex.lock();
	m_state = ProcessingState::Aborted;
	m_waitCond.wakeOne();
	m_mutex.unlock();
}

const QByteArray AudioEngine::getNextSample()
{
	QMutexLocker locker(&m_mutex);
	if (!m_segmentRead) {
		m_segmentRead = true;
		return QByteArray::fromRawData(m_buffer.constData() + m_readySegment * m_bufSegmentLen, m_bufSegmentLen);
	}
	return QByteArray::fromStdString("");
}

bool AudioEngine::updateAudioFormat()
{
	const int sampleRate = Preferences::getInstance().getSampleRate();
	const int bitsPerSample = Preferences::getInstance().getBitsPerSample();
	const int channelCount = Preferences::getInstance().getChannelCount();

	QMutexLocker locker(&m_mutex);
	// pcm 16-bit signed audio
	m_format.setSampleRate(sampleRate);
	m_format.setSampleSize(bitsPerSample);
	m_format.setChannelCount(channelCount);
	return true;
}

bool AudioEngine::updateBuffer()
{
	const int samples = Preferences::getInstance().getSamples();
	const int sampleSize = Preferences::getInstance().getSampleSize();

	QMutexLocker locker(&m_mutex);
	if (m_audioInput == nullptr || m_audioInput->state() == QAudio::StoppedState) {
		m_bufSegmentLen = samples * sampleSize;
		m_bufLen = m_bufSegmentLen * BUFFER_SEGMENTS;
		m_buffer.resize(m_bufLen);
		m_bufHead = 0;
		m_bufReadLen = 0;
		return true;
	}
	return false;
}

void AudioEngine::run()
{
	forever {
		QMutexLocker locker(&m_mutex);
		switch (m_state) {
		case Running:
			break;
		case Suspended:
			break;
		case Stopped:
			m_waitCond.wait(&m_mutex);
			break;
		case Aborted:
			return;
		}
		QThread::sleep(SLEEP_RATE);
	}
}

void AudioEngine::audioDataReady()
{
	m_mutex.lock();
	const qint64 bytesReady = m_audioInput->bytesReady();
	const qint64 bytesSpace = m_bufLen - m_bufHead;
	const qint64 bytesSpaceWrap = bytesReady - bytesSpace;
	qint64 bytesRead = 0;

	if (bytesSpaceWrap > 0) {
		bytesRead = m_ioDevice->read(m_buffer.data() + m_bufHead, bytesSpace) + m_ioDevice->read(m_buffer.data(), bytesSpaceWrap);
		m_bufHead = bytesSpaceWrap;
	}
	else {
		bytesRead = m_ioDevice->read(m_buffer.data() + m_bufHead, bytesReady);
		m_bufHead += bytesReady;
	}

	m_bufReadLen += bytesRead;

	if (m_bufReadLen >= m_bufSegmentLen) {
		m_segmentRead = false;
		m_readySegment = (m_readySegment + 1) % BUFFER_SEGMENTS;
		m_bufReadLen -= m_bufSegmentLen;
	}
	m_mutex.unlock();
}
