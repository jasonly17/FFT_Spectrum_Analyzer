#include "Preferences.h"

#include <QtCore/QtMath>

Preferences::Preferences()
	: m_power(12), m_sampleRate(44100), m_channelCount(2), m_bitsPerSample(16)
	, m_spectrumFudgeFactor(0.17)
	, m_bandsCount(64), m_bandWidth(6), m_bandHeight(200)
	, m_mirroring(false), m_reflection(false)
	, m_audioDeviceInfo(0)
{
}

Preferences & Preferences::getInstance()
{
	static Preferences instance;
	return instance;
}

const int Preferences::getPower()
{
	QMutexLocker locker(&m_mutex);
	return m_power;
}

void Preferences::setPower(int power)
{
	QMutexLocker locker(&m_mutex);
	m_power = power;
}

const int Preferences::getSamples()
{
	return qPow(2, getPower());
}

const int Preferences::getSampleRate()
{
	QMutexLocker locker(&m_mutex);
	return m_sampleRate;
}

void Preferences::setSampleRate(int sampleRate)
{
	QMutexLocker locker(&m_mutex);
	m_sampleRate = sampleRate;
}

const int Preferences::getChannelCount()
{
	QMutexLocker locker(&m_mutex);
	return m_channelCount;
}

void Preferences::setChannelCount(int channels)
{
	QMutexLocker locker(&m_mutex);
	m_channelCount = channels;
}

const int Preferences::getBitsPerSample()
{
	QMutexLocker locker(&m_mutex);
	return m_bitsPerSample;
}

void Preferences::setBitsPerSample(int bits)
{
	QMutexLocker locker(&m_mutex);
	m_bitsPerSample = bits;
}

const int Preferences::getBytesPerSample()
{
	return getBitsPerSample() / 8;
}

const int Preferences::getSampleSize()
{
	QMutexLocker locker(&m_mutex);
	return m_bitsPerSample / 8 * m_channelCount;
}

const double Preferences::getSpectrumFudgeFactor()
{
	QMutexLocker locker(&m_mutex);
	return m_spectrumFudgeFactor;
}

void Preferences::setSpectrumFudgeFactor(double fudgeFactor)
{
	QMutexLocker locker(&m_mutex);
	m_spectrumFudgeFactor = fudgeFactor;
}

const int Preferences::getBandsCount()
{
	QMutexLocker locker(&m_mutex);
	return m_bandsCount;
}

void Preferences::setBandsCount(int count)
{
	QMutexLocker locker(&m_mutex);
	m_bandsCount = count;
}

const int Preferences::getBandWidth()
{
	QMutexLocker locker(&m_mutex);
	return m_bandWidth;
}

void Preferences::setBandWidth(int width)
{
	QMutexLocker locker(&m_mutex);
	m_bandWidth = width;
}

const int Preferences::getBandHeight()
{
	QMutexLocker locker(&m_mutex);
	return m_bandHeight;
}

void Preferences::setBandHeight(int height)
{
	QMutexLocker locker(&m_mutex);
	m_bandHeight = height;
}

const bool Preferences::isMirroringEnabled()
{
	QMutexLocker locker(&m_mutex);
	return m_mirroring;
}

void Preferences::enableMirroring(bool state)
{
	QMutexLocker locker(&m_mutex);
	m_mirroring = state;
}

const bool Preferences::isReflectionEnabled()
{
	QMutexLocker locker(&m_mutex);
	return m_reflection;
}

void Preferences::enableReflection(bool state)
{
	QMutexLocker locker(&m_mutex);
	m_reflection = state;
}

void Preferences::setAudioInputDevice(int index)
{
	QMutexLocker locker(&m_mutex);
	m_audioDeviceInfo = index;
}

const int Preferences::getAudioInputDevice()
{
	QMutexLocker locker(&m_mutex);
	return m_audioDeviceInfo;
}
