#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QAudio>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioInput>
#include <QByteArray>
#include "spectrumanalyzer.h"

class QFile;
class QObject;

/******************************************************************************
 * Handles audio recording and management of the samples for analysis by the
 * frequency spectrum analyzer.
******************************************************************************/

class AudioEngine : public QObject
{
	Q_OBJECT

public:
	explicit AudioEngine(QObject *parent = 0);
	~AudioEngine();

	void startRecording();
	void stopRecording();

signals:
	void update(double val1, double val2, double val3);

public slots:
	void audioDataReady();

private:
	void calculateSpectrum(qint64 position);

private slots:
	void spectrumChanged(const FrequencySpectrum &spectrum);

private:
	QAudio::State mState;
	QAudioFormat mFormat;

	const QList<QAudioDeviceInfo> availableAudioInputDevices;
	QAudioDeviceInfo mAudioInputDeviceInfo;
	QAudioInput *mAudioInput;
	QIODevice *mAudioInputIODevice;

	QByteArray mBuffer;
	qint64 mBufferHead;
	qint64 mBufferTail;
	qint64 mBufferLength;
	qint64 mBufferDataLength;

	QByteArray mSpectrumBuffer;
	qint64 mSpectrumBufferLength;
	SpectrumAnalyzer mSpectrumAnalyzer;

	QFile *file;
	QDataStream out;
	int mCount;
};

#endif // AUDIOENGINE_H
