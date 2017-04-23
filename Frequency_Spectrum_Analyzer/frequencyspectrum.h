#ifndef FREQUENCYSPECTRUM_H
#define FREQUENCYSPECTRUM_H

#include <QObject>
#include <QVector>

/******************************************************************************
 * A container for storing frequency spectrum data.
******************************************************************************/

class FrequencySpectrum
{
public:
	FrequencySpectrum(int numPoints = 0);

	struct Sample {
		Sample() : amplitude(0.0) {}

		qreal amplitude; // range [0.0, 1.0]
	};

	Sample& operator[](int index);
	const Sample& operator[](int index) const;

private:
	QVector<Sample> mSpectrum;
};

#endif // FREQUENCYSPECTRUM_H
