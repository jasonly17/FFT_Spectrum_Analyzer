#pragma once
#include <QtCore/QThread>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioInput>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtCore/QIODevice>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>

class AudioEngine : public QThread
{
	Q_OBJECT
public:
	explicit AudioEngine(QObject *parent = 0);
	~AudioEngine();

	void start();
	void suspend();
	void resume();
	void stop();
	void terminate();
	
	const QByteArray getNextSample();
	bool updateAudioFormat();
	bool updateBuffer();

protected:
	void run();

private slots:
	void audioDataReady();

private:
	enum ProcessingState { Running, Suspended, Stopped, Aborted };

	const int BUFFER_SEGMENTS = 3; // 1 for ready and 2 for processing
	const int SLEEP_RATE = 2; // ms

	QMutex m_mutex;
	QWaitCondition m_waitCond;
	ProcessingState m_state;

	QAudioFormat m_format;
	QAudioInput *m_audioInput;
	QIODevice *m_ioDevice;

	QByteArray m_buffer;
	qint64 m_bufHead;
	qint64 m_bufLen;
	qint64 m_bufSegmentLen;
	qint64 m_bufReadLen;
	int m_readySegment;
	bool m_segmentRead;
};
