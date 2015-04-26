#include "audioengine.h"
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QTimer>
#include "def.h"
#include "frequencyspectrum.h"

AudioEngine::AudioEngine(QObject *parent)
	: QObject(parent)
	, mState(QAudio::StoppedState)
	, availableAudioInputDevices(QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
	, mAudioInputDeviceInfo(QAudioDeviceInfo::defaultInputDevice())
	, mBufferHead(0)
	, mBufferTail(0)
	, mBufferDataLength(0)
	, mCount(0)
{
	qRegisterMetaType<FrequencySpectrum>("FrequencySpectrum");

	mFormat.setSampleRate(SAMPLERATE);
	mFormat.setSampleSize(16);
	mFormat.setChannelCount(2);
	mFormat.setByteOrder(QAudioFormat::LittleEndian);
	mFormat.setSampleType(QAudioFormat::SignedInt);
	mFormat.setCodec("audio/pcm");

	mBufferLength = SAMPLES * BYTESPERSAMPLE * 4;
	mBuffer.resize(mBufferLength);

	mAudioInput = new QAudioInput(mAudioInputDeviceInfo, mFormat, this);
//	connect(mAudioInput, SIGNAL(stateChanged(QAudio::State)),
//			this, SLOT(audioStateChanged(QAudio::State)));

	connect(&mSpectrumAnalyzer, SIGNAL(spectrumChanged(const FrequencySpectrum&)),
			this, SLOT(spectrumChanged(const FrequencySpectrum&)));

	file = new QFile("out.raw");
	if (file->exists()) file->remove();
	file->open(QIODevice::Append);
	qDebug() << file->error();

//	QTimer::singleShot(120000, this, [this](){stopRecording();});
}

AudioEngine::~AudioEngine()
{
	stopRecording();
	mAudioInput->deleteLater();
	file->deleteLater();
}

/******************************************************************************
 * Public Functions
******************************************************************************/
void AudioEngine::startRecording()
{
	mAudioInputIODevice = mAudioInput->start();
	connect(mAudioInputIODevice, SIGNAL(readyRead()),
			this, SLOT(audioDataReady()));
}

void AudioEngine::stopRecording()
{
	qDebug() << "stopping";
	mAudioInput->stop();
	file->close();
	mAudioInputIODevice = 0;
}

/******************************************************************************
 * Public Slots
******************************************************************************/
void AudioEngine::audioDataReady()
{
	const qint64 bytesReady = mAudioInput->bytesReady();
	const qint64 bytesSpace = mBufferLength - mBufferHead;
	const qint64 bytesSpaceWrap = bytesReady - bytesSpace;
	qint64 bytesRead = 0;

	if (bytesSpaceWrap > 0) {
		bytesRead = mAudioInputIODevice->read(mBuffer.data() + mBufferHead, bytesSpace)
				+ mAudioInputIODevice->read(mBuffer.data(), bytesSpaceWrap);
		mBufferHead = bytesSpaceWrap;
	} else {
		bytesRead = mAudioInputIODevice->read(mBuffer.data() + mBufferHead, bytesReady);
		mBufferHead += bytesReady;
	}

	if (bytesRead) mBufferDataLength += bytesRead;

	if (mBufferDataLength >= SAMPLES * BYTESPERSAMPLE) {
//		qDebug() << "writing" << mBufferTail
//				 << mBufferTail + SAMPLES * BYTESPERSAMPLE;

//		char *ptr = mBuffer.data() + mBufferTail;
//		const char *end = ptr + BUFFERSIZE;
//		while (ptr < end) {
//			qint16 *value = reinterpret_cast<qint16*>(ptr);
//			*value = *value * 1; // relative gain
//			ptr += 2;
//		}

		calculateSpectrum(mBufferTail);
		file->write(mBuffer.data() + mBufferTail, SAMPLES * BYTESPERSAMPLE);
		mBufferDataLength -= SAMPLES * BYTESPERSAMPLE;
		mBufferTail += SAMPLES * BYTESPERSAMPLE;
		if (mBufferTail >= mBufferLength) {
			mBufferTail = 0;
		}
	}

//	qDebug() << "head:" << mBufferHead << "tail:" << mBufferTail;
//	qDebug() << "bytes ready:" << bytesReady << "bytes space:" << bytesSpace
//			 << "data in buffer:" << mBufferDataLength;
}

/******************************************************************************
 * Private
******************************************************************************/
void AudioEngine::calculateSpectrum(qint64 position)
{
	if (mSpectrumAnalyzer.isReady()) {
		mSpectrumBuffer = QByteArray::fromRawData(mBuffer.constData() + position,
												  SAMPLES * BYTESPERSAMPLE);
		mSpectrumAnalyzer.calculateSpectrum(mSpectrumBuffer);
	}
}

/******************************************************************************
 * Private Slots
******************************************************************************/
void AudioEngine::spectrumChanged(const FrequencySpectrum &spectrum)
{
	const FrequencySpectrum::Sample s1 = spectrum[2];
	const FrequencySpectrum::Sample s2 = spectrum[4];
	const FrequencySpectrum::Sample s3 = spectrum[6];
	emit update(s1.amplitude * 200, s2.amplitude * 200, s3.amplitude * 200);
}
