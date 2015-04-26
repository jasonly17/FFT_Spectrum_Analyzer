#include "spectrumanalyzer.h"
#include <QDebug>
#include <QtMath>
#include <QMetaType>
#include <QThread>
#include "def.h"
#include "ffftwrapper.h"
#include "utils.h"

SpectrumAnalyzerThread::SpectrumAnalyzerThread(QObject *parent)
	: QObject(parent)
	, mFFT(new FFFTWrapper)
	, mWindow(SAMPLES, 0.0)
	, mInput(SAMPLES, 0.0)
	, mOutput(SAMPLES, 0.0)
	, mSpectrum(SAMPLES)
	, mThread(new QThread(this))
{
	setParent(0);
	moveToThread(mThread);
	mThread->start();

	for (int i = 0; i < SAMPLES; ++i) {
		DataType x = 0.5 * (1 - qCos((2 * M_PI * i) / (SAMPLES - 1)));
		mWindow[i] = x;
	}
}

SpectrumAnalyzerThread::~SpectrumAnalyzerThread()
{ delete mFFT; }

void SpectrumAnalyzerThread::calculateSpectrum(const QByteArray &buffer, int inputFrequency)
{
	const char *ptr = buffer.constData();
	for (int i = 0; i < SAMPLES; ++i) {
		const qint16 pcmSample = *reinterpret_cast<const qint16*>(ptr);
		const DataType realSample = pcmToReal(pcmSample);
		const DataType windowedSample = realSample * mWindow[i];
		mInput[i] = windowedSample;
		ptr += BYTESPERSAMPLE;
	}

	mFFT->calculateFFT(mOutput.data(), mInput.data());

	for (int i = 2; i <= SAMPLES / 2; i++) {
		mSpectrum[i].frequency = qreal(i * inputFrequency) / SAMPLES * 2;
		const qreal real = mOutput[i];
		qreal imag = 0.0;
		if (i < SAMPLES / 2) imag = mOutput[SAMPLES / 2 + i];

		const qreal magnitude = sqrt(real * real + imag * imag);
		qreal amplitude = SPECTRUMFUDGEFACTOR * log(magnitude);

		mSpectrum[i].clipped = amplitude > 1.0;
		mSpectrum[i].amplitude = qMin(qreal(1.0), qMax(qreal(0.0), amplitude));
	}
	emit calculationComplete(mSpectrum);
}


SpectrumAnalyzer::SpectrumAnalyzer(QObject *parent)
	: QObject(parent)
	, mThread(new SpectrumAnalyzerThread(this))
	, mState(Idle)
{
	connect(mThread, SIGNAL(calculationComplete(FrequencySpectrum)),
			this, SLOT(calculationComplete(FrequencySpectrum)));
}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
}

void SpectrumAnalyzer::calculateSpectrum(const QByteArray &buffer)
{
	if (isReady()) {
		mState = Busy;
		QMetaObject::invokeMethod(mThread, "calculateSpectrum",
								  Qt::AutoConnection,
								  Q_ARG(QByteArray, buffer),
								  Q_ARG(int, SAMPLERATE));
	}
}

bool SpectrumAnalyzer::isReady() const
{ return (mState == Idle); }

void SpectrumAnalyzer::cancelCalculation()
{
	if (mState == Busy) mState = Cancelled;
}

void SpectrumAnalyzer::calculationComplete(const FrequencySpectrum &spectrum)
{
	if (mState == Busy) emit spectrumChanged(spectrum);
	mState = Idle;
}
