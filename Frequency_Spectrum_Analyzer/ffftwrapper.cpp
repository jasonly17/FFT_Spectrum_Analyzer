#include "ffftwrapper.h"

#if defined Q_CC_MSVC
#    pragma warning(disable:4100)
#elif defined Q_CC_GNU
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined Q_CC_MWERKS
#    pragma warning off (10182)
#endif

#include "def.h"
#include "FFTRealFixLen.h"

class FFFTWrapperPrivate
{
public:
	ffft::FFTRealFixLen<POWER> mFFT;
};


FFFTWrapper::FFFTWrapper()
	: mPrivateFFT(new FFFTWrapperPrivate)
{
}

FFFTWrapper::~FFFTWrapper()
{
	delete mPrivateFFT;
}

void FFFTWrapper::calculateFFT(DataType out[], const DataType in[])
{
	mPrivateFFT->mFFT.do_fft(out, in);
}
