#ifndef DEF_H
#define DEF_h

static const int POWER = 12;
static const int SAMPLES = 4096; // 2^POWER ~ higher than 4096 causes noticable lag
static const int SAMPLERATE = 44100;
static const int CHANNELCOUNT = 2;
static const int BITSPERSAMPLE = 16;
static const int BYTESPERSAMPLE = BITSPERSAMPLE / 8;
static const int SAMPLESIZE = BYTESPERSAMPLE * CHANNELCOUNT;
static const double SPECTRUMFUDGEFACTOR = 0.12;
static const int BANDS = 100; // noticable lag > 100 bands
static const bool MIRROR = false;
static const bool REFLECTION = true;

#endif // DEF_H
