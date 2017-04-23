#include "audioengine.h"
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QTimer>
#include "def.h"
#include "frequencyspectrum.h"

/******************************************************************************
 * AudioEngine
******************************************************************************/
AudioEngine::AudioEngine(QObject *parent)
	: QObject(parent)
	, mBufferHead(0)
	, mBufferTail(0)
	, mBufferDataLength(0)
//	, mCount(0)
{
	qRegisterMetaType<FrequencySpectrum>("FrequencySpectrum");

	// pcm 16-bit signed audio
	QAudioFormat mFormat;
	mFormat.setSampleRate(SAMPLERATE);
	mFormat.setSampleSize(BITSPERSAMPLE);
	mFormat.setChannelCount(CHANNELCOUNT);
	mFormat.setByteOrder(QAudioFormat::LittleEndian);
	mFormat.setSampleType(QAudioFormat::SignedInt);
	mFormat.setCodec("audio/pcm");

	// give extra buffers for simultaneous buffering and processing
    mBufferLength = SAMPLES * SAMPLESIZE * 4;
	mBuffer.resize(mBufferLength);

	qDebug() << QAudioDeviceInfo::defaultInputDevice().deviceName();

	//mAudioInput = new QAudioInput(QAudioDeviceInfo::availableDevices(QAudio::AudioInput).at(21), mFormat, this);
	mAudioInput = new QAudioInput(QAudioDeviceInfo::defaultInputDevice(), mFormat, this);
//    mAudioInput->setBufferSize(10000);
//    mAudioInput->setBufferSize(5760);

	connect(&mSpectrumAnalyzer, SIGNAL(spectrumChanged(const FrequencySpectrum&)),
			this, SIGNAL(spectrumChanged(const FrequencySpectrum&)));

    resetTimer = new QTimer();
    resetTimer->setInterval(5000);
    resetTimer->setSingleShot(false);

    connect(resetTimer, SIGNAL(timeout()),
            this, SLOT(reset()));

    resetTimer->start();
}

AudioEngine::~AudioEngine()
{
	stopRecording();
	mAudioInput->deleteLater();
}

/*********** Public Functions ***********/
// Starts recording audio from the default input device into the buffer.
void AudioEngine::startRecording()
{
	mAudioInputIODevice = mAudioInput->start();
	qDebug() << "Buffer size:" << mAudioInput->bufferSize();
	connect(mAudioInputIODevice, SIGNAL(readyRead()),
			this, SLOT(audioDataReady()));
}

// Stops recording audio and closes the raw output file.
void AudioEngine::stopRecording()
{
	qDebug() << "stopping";
	mAudioInput->stop();
	mAudioInputIODevice = 0;
}

/*********** Public Slots ***********/
// Loads the audio data that is ready into the buffer and processes it if the
// buffer is at least the FFT size.
void AudioEngine::audioDataReady()
{
	const qint64 bytesReady = mAudioInput->bytesReady();
	const qint64 bytesSpace = mBufferLength - mBufferHead;
	const qint64 bytesSpaceWrap = bytesReady - bytesSpace;
	qint64 bytesRead = 0;

	// treats buffer like a ring buffer
	if (bytesSpaceWrap > 0) {
		bytesRead = mAudioInputIODevice->read(mBuffer.data() + mBufferHead, bytesSpace)
				+ mAudioInputIODevice->read(mBuffer.data(), bytesSpaceWrap);
		mBufferHead = bytesSpaceWrap;
	} else {
		bytesRead = mAudioInputIODevice->read(mBuffer.data() + mBufferHead, bytesReady);
		mBufferHead += bytesReady;
	}

	if (bytesRead) mBufferDataLength += bytesRead;

//	if (mBufferDataLength >= SAMPLES * SAMPLESIZE / 2) {
//		calculateSpectrum(mBufferTail);
//		mBufferDataLength -= SAMPLES * SAMPLESIZE / 2;
//		mBufferTail += SAMPLES * SAMPLESIZE / 2;
//		if (mBufferTail >= mBufferLength) {
//			mBufferTail = 0;
//		}
//	}

    if (mBufferDataLength >= SAMPLES * SAMPLESIZE) {
//		char *ptr = mBuffer.data() + mBufferTail;
//		const char *end = ptr + BUFFERSIZE;
//		while (ptr < end) {
//			qint16 *value = reinterpret_cast<qint16*>(ptr);
//			*value = *value * 1; // relative gain
//			ptr += 2;
//		}

		calculateSpectrum(mBufferTail);
        mBufferDataLength -= SAMPLES * SAMPLESIZE;
        mBufferTail += SAMPLES * SAMPLESIZE;
		if (mBufferTail >= mBufferLength) {
			mBufferTail = 0;
		}
	}
}

void AudioEngine::reset()
{
    if (mAudioInputIODevice != 0) {
        mBufferHead = 0;
        mBufferTail = 0;
        mBufferDataLength = 0;
    }
}

/*********** Private Functions ***********/
// Send the audio data to be processed by the spectrum analyzer.
void AudioEngine::calculateSpectrum(qint64 position)
{
//	if (mSpectrumAnalyzer.isReady()) {
//		int index = position + SAMPLES * SAMPLESIZE;
//		if (index > mBuffer.size()) {
//			int remLength = index - mBuffer.size();
//			mSpectrumBuffer = QByteArray::fromRawData(mBuffer.constData() + position, remLength);
//			mSpectrumBuffer.append(QByteArray::fromRawData(mBuffer.constData(), SAMPLES * SAMPLESIZE - remLength));
//		} else {
//			mSpectrumBuffer = QByteArray::fromRawData(mBuffer.constData() + position,
//													  SAMPLES * SAMPLESIZE);
//		}
//		mSpectrumAnalyzer.calculateSpectrum(mSpectrumBuffer);
//	}

	if (mSpectrumAnalyzer.isReady()) {
		mSpectrumBuffer = QByteArray::fromRawData(mBuffer.constData() + position,
                                                  SAMPLES * SAMPLESIZE);
		mSpectrumAnalyzer.calculateSpectrum(mSpectrumBuffer);
	}
}
