#ifndef FFFTWRAPPER_H
#define FFFTWRAPPER_H

class FFFTWrapperPrivate;

class FFFTWrapper
{
public:
	FFFTWrapper();
	~FFFTWrapper();

	typedef float DataType;
	void calculateFFT(DataType out[], const DataType in[]);

private:
	FFFTWrapperPrivate*  mPrivateFFT;
};

#endif // FFFTWRAPPER_H
