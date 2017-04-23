#ifndef DEF_H
#define DEF_H

#include <QtMath>

static const int POWER = 12; // 10: 1024, 11: 2048, 12: 4096
static const int SAMPLES = qPow(2, POWER);//4096; // 2^POWER ~ higher than 4096 causes noticable lag
static const int SAMPLERATE = 48000;
static const int CHANNELCOUNT = 2;
static const int BITSPERSAMPLE = 16;
static const int BYTESPERSAMPLE = BITSPERSAMPLE / 8;
static const int SAMPLESIZE = BYTESPERSAMPLE * CHANNELCOUNT;
static const double SPECTRUMFUDGEFACTOR = 0.17;
static const int BANDS = 160;
static const bool MIRROR = false;
static const bool REFLECTION = false;
static const int BANDWIDTH = 6;
static const int BANDHEIGHT = 200;

#endif // DEF_H
