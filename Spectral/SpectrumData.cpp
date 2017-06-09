#include "SpectrumData.h"

SpectrumData::SpectrumData(int pointsCount)
	: m_spectrumData(pointsCount)
{
}

SpectrumData::~SpectrumData()
{
}

SpectrumData::Frequency& SpectrumData::operator[](const int index)
{
	return m_spectrumData[index];
}

const SpectrumData::Frequency& SpectrumData::operator[](const int index) const
{
	return m_spectrumData[index];
}