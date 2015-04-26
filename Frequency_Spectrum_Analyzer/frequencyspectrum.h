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
		Sample() : frequency(0.0), amplitude(0.0), phase(0.0), clipped(false) {}

		qreal frequency; // Hz
		qreal amplitude; // range [0.0, 1.0]
		qreal phase; // range [0.0, 2*pi]
		bool clipped; // indicates whether sample has been clipped during spectrum analysis
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
