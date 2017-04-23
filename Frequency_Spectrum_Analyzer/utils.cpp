#include "utils.h"
#include <QAudioFormat>

/******************************************************************************
 * Utility Functions
******************************************************************************/

// Returns the duration of the audio depending on bytes
// return: microseconds
//qint64 audioDuration(const QAudioFormat &format, qint64 bytes)
//{
//	return (bytes * 1000000) /
//		(format.sampleRate() * format.channelCount() * (format.sampleSize() / 8));
//}

// Returns the size of the audio depending on microSeconds
// return: bytes
//qint64 audioLength(const QAudioFormat &format, qint64 microSeconds)
//{
//	qint64 result = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
//			* microSeconds / 1000000;
//	result -= result % (format.channelCount() * format.sampleSize());
//	return result;
//}

// Scales pcm to range [-1.0, 1.0]
qreal pcmToReal(qint16 pcm)
{ return qreal(pcm) / 32768; }
