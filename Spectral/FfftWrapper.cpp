#include "FFFTWrapper.h"

#include <exception>

#include "ffft/FFTRealFixLen.h"

/// Special exception for invalid fft power.
class InvalidFFTPowerException : public std::exception
{
	virtual const char *what() const throw()
	{
		return "Invalid fft power.";
	}
};

/// Container to hold the fft object.
class FFFTWrapperPrivate
{
public:
	FFFTWrapperPrivate(const int power)
	{
		createFFT(power);
	}

	~FFFTWrapperPrivate()
	{
		if (m_fft)
			delete m_fft;
	}

	/// <remark>power needs to be between 6 and 13 inclusive.</remark>
	void ChangePower(const int power)
	{
		createFFT(power);
	}

	void PerformFFT(const FFFTWrapper::DataType in[], FFFTWrapper::DataType out[])
	{
		switch (m_power) {
		case 6: // 64
			static_cast<ffft::FFTRealFixLen<6> *>(m_fft)->do_fft(out, in);
			break;
		case 7: // 128
			static_cast<ffft::FFTRealFixLen<7> *>(m_fft)->do_fft(out, in);
			break;
		case 8: // 256
			static_cast<ffft::FFTRealFixLen<8> *>(m_fft)->do_fft(out, in);
			break;
		case 9: // 512
			static_cast<ffft::FFTRealFixLen<9> *>(m_fft)->do_fft(out, in);
			break;
		case 10: // 1024
			static_cast<ffft::FFTRealFixLen<10> *>(m_fft)->do_fft(out, in);
			break;
		case 11: // 2048
			static_cast<ffft::FFTRealFixLen<11> *>(m_fft)->do_fft(out, in);
			break;
		case 12: // 4096
			static_cast<ffft::FFTRealFixLen<12> *>(m_fft)->do_fft(out, in);
			break;
		case 13: // 8192
			static_cast<ffft::FFTRealFixLen<13> *>(m_fft)->do_fft(out, in);
			break;
		}
	}

private:
	void *createFFT(const int power)
	{
		if (m_fft)
			delete m_fft;

		m_power = power;
		switch (m_power) {
		case 6: // 64
			m_fft = new ffft::FFTRealFixLen<6>;
			break;
		case 7: // 128
			m_fft = new ffft::FFTRealFixLen<7>;
			break;
		case 8: // 256
			m_fft = new ffft::FFTRealFixLen<8>;
			break;
		case 9: // 512
			m_fft = new ffft::FFTRealFixLen<9>;
			break;
		case 10: // 1024
			m_fft = new ffft::FFTRealFixLen<10>;
			break;
		case 11: // 2048
			m_fft = new ffft::FFTRealFixLen<11>;
			break;
		case 12: // 4096
			m_fft = new ffft::FFTRealFixLen<12>;
			break;
		case 13: // 8192
			m_fft = new ffft::FFTRealFixLen<13>;
			break;
		default:
			throw new InvalidFFTPowerException();
		}
	}

	int m_power;
	void *m_fft = 0;
};


FFFTWrapper::FFFTWrapper(const int power)
	: m_fft(new FFFTWrapperPrivate(power))
{
}

FFFTWrapper::~FFFTWrapper()
{
	delete m_fft;
}

void FFFTWrapper::calculateFFT(const DataType in[], DataType out[])
{
	m_fft->PerformFFT(in, out);
}

void FFFTWrapper::changePower(const int power)
{
	m_fft->ChangePower(power);
}
