#include "frequencyspectrum.h"

FrequencySpectrum::FrequencySpectrum(int numPoints)
	: mSpectrum(numPoints)
{
}

/******************************************************************************
 * Public Functions
******************************************************************************/
// Resets all samples currently stored in mSpectrum.
void FrequencySpectrum::reset()
{
	Iterator i = begin();
	while (i != end()) *i++ = Sample();
}

// Returns the number of samples currently stored in mSpectrum.
int FrequencySpectrum::count() const
{ return mSpectrum.count(); }

// Convenience operator for accessing elements in mSpectrum.
FrequencySpectrum::Sample& FrequencySpectrum::operator[](int index)
{ return mSpectrum[index]; }

// Convenience operator for accessing elements in mSpectrum.
const FrequencySpectrum::Sample& FrequencySpectrum::operator[](int index) const
{ return mSpectrum[index]; }

// Returns an iterator to the beginning of mSpectrum.
FrequencySpectrum::Iterator FrequencySpectrum::begin()
{ return mSpectrum.begin(); }

// Returns an iterator to the end of mSpectrum.
FrequencySpectrum::Iterator FrequencySpectrum::end()
{ return mSpectrum.end(); }

// Returns an iterator to the beginning of mSpectrum.
FrequencySpectrum::ConstIterator FrequencySpectrum::begin() const
{ return mSpectrum.begin(); }

// Returns an iterator to the end of mSpectrum.
FrequencySpectrum::ConstIterator FrequencySpectrum::end() const
{ return mSpectrum.end(); }
