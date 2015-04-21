#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QAudio>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioInput>
#include <QByteArray>

class QFile;
class QObject;

class AudioEngine : public QObject
{
	Q_OBJECT

public:
	explicit AudioEngine(QObject *parent = 0);
	~AudioEngine();

	void startRecording();
	void stopRecording();

public slots:
	void audioDataReady();

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

	QFile *file;
	QDataStream out;
	int mCount;
};




























//#include "spectrum.h"
//#include "spectrumanalyser.h"

//#include <QAudioDeviceInfo>
//#include <QAudioFormat>
//#include <QBuffer>
//#include <QByteArray>
//#include <QDir>
//#include <QObject>
//#include <QVector>

////class FrequencySpectrum;
//class QAudioInput;

//class AudioEngine : public QObject
//{
//	Q_OBJECT

//public:
//	explicit AudioEngine(QObject *parent = 0);
//	~AudioEngine();

//	const QList<QAudioDeviceInfo>& availableAudioInputDevices() const;
//	QAudio::State state() const;

//	void reset();

//	qint64 recordPosition() const;
//	qint64 bufferLength() const;
//	qint64 dataLength() const;

////	void setWindowFunction(WindowFunction type);

//public slots:
//	void startRecording();
//	void suspend();
//	void setAudioInputDevice(const QAudioDeviceInfo &device);

//signals:
//	void stateChanged(QAudio::State state);
//	void dataLengthChanged(qint64 duration);
//	void recordPositionChanged(qint64 position);
////	void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum);
//	void bufferChanged(qint64 position, qint64 length, const QByteArray &buffer);

//private slots:
//	void audioNotify();
//	void audioStateChanged(QAudio::State state);
//	void audioDataReady();
////	void spectrumChanged(const FrequencySpectrum &spectrum);

//private:
//	void stopRecording();
//	void setState(QAudio::State state);
//	void setRecordPosition(qint64 position, bool forceEmit = false);
////	void calculateSpectrum(qint64 position);

//private:
//	QAudio::State mState;
//	QAudioFormat mFormat;

//	const QList<QAudioDeviceInfo> mAvailableAudioInputDevices;
//	QAudioDeviceInfo mAudioInputDeviceInfo;
//	QAudioInput *mAudioInput;
//	QIODevice *mAudioInputIODevice;
//	qint64 mRecordPosition;

//	QByteArray mBuffer;
//	qint64 mBufferHead;
//	qint64 mBufferTail;
//	qint64 mBufferLength;
//	qint64 mDataLength;

//	int mSpectrumBufferLength;
//	QByteArray mSpectrumBuffer;
////	SpectrumAnalyser mSpectrumAnalyser;
//	qint64 mSpectrumPosition;

//	int mCount;
//};



























//#include <QAudio>
//#include <QAudioDeviceInfo>
//#include <QObject>
//#include "audiobuffer.h"

//class QAudioInput;
//class QFile;

//class AudioEngine : public QObject
//{
//	Q_OBJECT

//public:
//	explicit AudioEngine(QObject *parent = 0);
//	~AudioEngine();

//	const QList<QAudioDeviceInfo>& availableAudioInputDevices() const;
//	QAudio::State state() const;
//	qint64 bufferLength() const;

//public slots:
//	void startRecording();
//	void suspend();
//	void setAudioInputDevice(const QAudioDeviceInfo &device);

//signals:
//	void stateChanged(QAudio::State state);

//private slots:
//	void audioStateChanged(QAudio::State state);
//	void audioDataReady();

//private:
//	void stopRecording();
//	void setState(QAudio::State state);

//private:
//	const QList<QAudioDeviceInfo> mAvailAudioInputDevices;
//	QAudioDeviceInfo mAudioInputDeviceInfo;

//	QFile *mFile;
//	QAudio::State mState;
//	QAudioFormat format;
//	QAudioInput *mAudioInput;
//	AudioBuffer mBuffer;
//	qint64 mBufferPosition;
//	qint64 mBufferLength;
//	qint64 mDataLength;

//	int mCount;
//};





//class AudioEngine : public QObject
//{
//	Q_OBJECT

//public:
//	explicit AudioEngine(QObject *parent = 0);
//	~AudioEngine();

//	const QList<QAudioDeviceInfo>& availableAudioInputDevices() const;
//	QAudio::State state() const;
//	double rmsLevel() const;
//	double peakLevel() const;
//	qint64 bufferLength() const;

//public slots:
//	void startRecording();
//	void suspend();
//	void setAudioInputDevice(const QAudioDeviceInfo &device);

//signals:
//	// Audio state has changed
//	void stateChanged(QAudio::State state);

//	// Audio level has changed
//	// param rmsLevel: RMS level in range [0.0, 1.0]
//	// param peakLevel: peak level in range [0.0, 1.0]
//	// param numSamples: number of audio samples analyzed
//	void levelChanged(double rmsLevel, double peakLevel, int numSamples);

//	// Audio level has changed
//	// param position: position of start of window in bytes
//	// param length: length of window in bytes
//	// param spectrum: resulting frequency spectrum
////	void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum);

//	// Buffer containing audio data has changed
//	// param position: position of the start of the buffer in bytes
//	// param length: length of the buffer in bytes
//	// param buffer: the buffer itself
//	void bufferChanged(qint64 position, qint64 length, const QByteArray &buffer);


//private slots:
//	void audioNotify();
//	void audioStateChanged(QAudio::State state);
//	void audioDataReady();

//	// Handles when the frequency spectrum has changed
////	void spectrumChanged(const FrequencySpectrum &spectrum);

//private:
//	// Initializes settings for current audio input device
//	bool initialize();

//	// Stops recording from the audio input IO device
//	void stopRecording();

//	// Sets the audio state to state
//	void setState(QAudio::State state);

//	// Sets the record position to position and them force emits a signal depending
//	// on forceEmit
//	// param position: bytes
//	void setRecordPosition(qint64 position, bool forceEmit = false);

//	// Calculates the audio level at position
//	// param position: bytes
//	// param length: length of buffer in bytes
//	void calcLevel(qint64 position, qint64 length);

//	// Calculates the frequency spectrum at position
//	// param position: bytes
//	void calcSpectrum(qint64 position);

//	// Sets the audio level according to the most recently processed samples
//	// param rmsLevel: RMS level in range [0.0, 1.0]
//	// param peakLevel: peak level in range [0.0, 1.0]
//	// param numSamples: number of audio samples analyzed
//	void setLevel(double rmsLevel, double peakLevel, int numSamples);

//private:
//	const QList<QAudioDeviceInfo> mAvailAudioInputDevices;
//	QAudioDeviceInfo mAudioInputDeviceInfo;

//	QAudio::State mState;
//	QAudioInput *mAudioInput;
//	QIODevice *mAudioInputIODevice;

//	AudioBuffer mBuffer;
//	qint64 mBufferPosition;
//	qint64 mBufferLength;
//	qint64 mDataLength;

//	int mLevelBufferLength;
//	double mRMSLevel;
//	double mPeakLevel;

//	AudioBuffer mSpectrumBuffer;
//	qint64 mSpectrumPosition;
//	int mSpectrumBufferLength;
//	//SpectrumAnalyzer spectrumAnalyzer

//	int mCount;
//};

#endif // AUDIOENGINE_H
