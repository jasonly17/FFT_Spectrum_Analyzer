#include "audioengine.h"
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QTimer>

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
	mFormat.setSampleRate(44100);
	mFormat.setSampleSize(16);
	mFormat.setChannelCount(2);
	mFormat.setByteOrder(QAudioFormat::LittleEndian);
	mFormat.setSampleType(QAudioFormat::SignedInt);
	mFormat.setCodec("audio/pcm");

	mBufferLength = 8192 * 10;
	mBuffer.resize(mBufferLength + 1);

	mAudioInput = new QAudioInput(mAudioInputDeviceInfo, mFormat, this);
//	connect(mAudioInput, SIGNAL(stateChanged(QAudio::State)),
//			this, SLOT(audioStateChanged(QAudio::State)));
//	connect(mAudioInput, SIGNAL(notify()),
//			this, SLOT(audioNotify()));

	file = new QFile("output/out.raw");
	if (file->exists()) file->remove();
	file->open(QIODevice::Append);

	QTimer::singleShot(30000, this, [this](){stopRecording();});
}

AudioEngine::~AudioEngine()
{
	mAudioInput->deleteLater();
	file->deleteLater();
}

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
	exit(0);
}

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
		qDebug() << bytesRead;
	} else {
		bytesRead = mAudioInputIODevice->read(mBuffer.data() + mBufferHead, bytesReady);
		mBufferHead += bytesReady;
	}

	if (bytesRead) {
		mBufferDataLength += bytesRead;
	}

	if (mBufferDataLength >= 8192) {
		qDebug() << "writing" << mBufferTail << mBufferTail + 8192;
		file->write(mBuffer.data() + mBufferTail, 8192);
		mBufferDataLength -= 8192;
		mBufferTail += 8192;
		if (mBufferTail >= mBufferLength) {
			mBufferTail = 0;
		}
	}

	qDebug() << "head:" << mBufferHead << "tail:" << mBufferTail;
	qDebug() << "bytes ready:" << bytesReady << "bytes space:" << bytesSpace
			 << "data in buffer:" << mBufferDataLength;
}
