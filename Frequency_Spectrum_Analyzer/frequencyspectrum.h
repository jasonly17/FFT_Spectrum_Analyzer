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
		Sample() : frequency(0.0), amplitude(0.0) {}

		qreal frequency; // Hz
		qreal amplitude; // range [0.0, 1.0]
	};

	typedef QVector<Sample>::Iterator Iterator;
	typedef QVector<Sample>::ConstIterator ConstIterator;

	void reset();
	int count() const;
	Sample& operator[](int index);
	const Sample& operator[](int index) const;
	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

private:
	QVector<Sample> mSpectrum;
};

#endif // FREQUENCYSPECTRUM_H
