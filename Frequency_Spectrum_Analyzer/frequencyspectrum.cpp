#include "frequencyspectrum.h"

/******************************************************************************
 * FrequencySpectrum
******************************************************************************/
FrequencySpectrum::FrequencySpectrum(int numPoints)
	: mSpectrum(numPoints)
{
}

/*********** Public Functions ***********/
// Convenience operator for accessing elements in mSpectrum.
FrequencySpectrum::Sample& FrequencySpectrum::operator[](int index)
{ return mSpectrum[index]; }

// Convenience operator for accessing elements in mSpectrum.
const FrequencySpectrum::Sample& FrequencySpectrum::operator[](int index) const
{ return mSpectrum[index]; }
