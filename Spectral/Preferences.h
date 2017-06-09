#pragma once
#include <QtCore/QMutex>
#include <QtMultimedia/QAudioDeviceInfo>

class Preferences
{
public:
	static Preferences& getInstance();

	Preferences(Preferences const&) = delete;

	void operator=(Preferences const&) = delete;

	const int getPower();
	void setPower(int power);
	const int getSamples();
	const int getSampleRate();
	void setSampleRate(int sampleRate);
	const int getChannelCount();
	void setChannelCount(int channels);
	const int getBitsPerSample();
	void setBitsPerSample(int bits);
	const int getBytesPerSample();
	const int getSampleSize();

	const double getSpectrumFudgeFactor();
	void setSpectrumFudgeFactor(double fudgeFactor);
	const int getBandsCount();
	void setBandsCount(int count);
	const int getBandWidth();
	void setBandWidth(int width);
	const int getBandHeight();
	void setBandHeight(int height);

	const bool isMirroringEnabled();
	void enableMirroring(bool state);
	const bool isReflectionEnabled();
	void enableReflection(bool state);

	void setAudioInputDevice(int index);
	const int getAudioInputDevice();

private:
	Preferences();

	QMutex m_mutex;
	int m_power, m_sampleRate, m_channelCount, m_bitsPerSample;
	double m_spectrumFudgeFactor;
	int m_bandsCount, m_bandWidth, m_bandHeight;
	bool m_mirroring, m_reflection;
	int m_audioDeviceInfo;
};
