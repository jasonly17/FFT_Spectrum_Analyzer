#include "ffftwrapper.h"

// Trying to suppress warnings but not working.
#if defined Q_CC_MSVC
#    pragma warning(disable:4100)
#elif defined Q_CC_GNU
#    pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined Q_CC_MWERKS
#    pragma warning off (10182)
#endif

#include "def.h"
#include "FFTRealFixLen.h"

// Private class containing the FFT object
class FFFTWrapperPrivate
{
public:
	ffft::FFTRealFixLen<POWER> mFFT;
};

/******************************************************************************
 * FFFTWrapper
******************************************************************************/
FFFTWrapper::FFFTWrapper()
	: mPrivateFFT(new FFFTWrapperPrivate)
{
}

FFFTWrapper::~FFFTWrapper()
{
	delete mPrivateFFT;
}

/*********** Public Functions ***********/
// Performs the FFT on the data provided by in.
void FFFTWrapper::calculateFFT(DataType out[], const DataType in[])
{
	mPrivateFFT->mFFT.do_fft(out, in);
}
