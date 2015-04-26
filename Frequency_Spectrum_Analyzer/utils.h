#ifndef UTILS_H
#define UTILS_H

#include <QtGlobal>

class QAudioFormat;

qint64 audioDuration(const QAudioFormat &format, qint64 bytes);
qint64 audioLength(const QAudioFormat &format, qint64 microSeconds);
qreal nyquistFrequency(const QAudioFormat &format);
qreal pcmToReal(qint16 pcm);
qint16 realToPcm(qreal real);

#endif // UTILS_H
