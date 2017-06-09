#pragma once
class FFFTWrapperPrivate;

/// Wrapper for the FFTReal library.
class FFFTWrapper
{
public:
	FFFTWrapper(const int power);
	~FFFTWrapper();

	typedef float DataType;

	void calculateFFT(const DataType in[], DataType out[]);
	void changePower(const int power);

private:
	FFFTWrapperPrivate *m_fft;
};
