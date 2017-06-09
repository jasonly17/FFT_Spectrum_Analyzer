#pragma once
#include <QtCore/QtMath>
#include <QtCore/QVector>

class SpectrumData
{
public:
	explicit SpectrumData(int pointsCount);
	~SpectrumData();

	struct Frequency
	{
		qreal m_amplitude;
	};

	Frequency& operator[](const int index);
	const Frequency& operator[](const int index) const;

private:
	QVector<Frequency> m_spectrumData;
};
