#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include <QByteArray>
#include <QObject>
#include <QVector>
#include "frequencyspectrum.h"

class FFFTWrapper;

/******************************************************************************
 * Class to perform the spectrum analysis in a separate thread.
******************************************************************************/

class SpectrumAnalyzerThread : public QObject
{
	Q_OBJECT

public:
	SpectrumAnalyzerThread(QObject *parent);
	~SpectrumAnalyzerThread();

public slots:
	void calculateSpectrum(const QByteArray &buffer);

signals:
	void calculationComplete(const FrequencySpectrum &spectrum);

private:
	void calculateWindow();

private:
	FFFTWrapper *mFFT;
	typedef float DataType;
	QVector<DataType> mWindow;
	QVector<DataType> mInput;
	QVector<DataType> mOutput;
	FrequencySpectrum mSpectrum;
	QThread *mThread;
};

/******************************************************************************
 * Handles the spectrum analysis of the audio data provided by the audio engine.
******************************************************************************/

class SpectrumAnalyzer : public QObject
{
	Q_OBJECT

public:
	explicit SpectrumAnalyzer(QObject *parent = 0);
	~SpectrumAnalyzer();

	void calculateSpectrum(const QByteArray &buffer);
	bool isReady() const;
	void cancelCalculation();

signals:
	void spectrumChanged(const FrequencySpectrum &spectrum);

private slots:
	void calculationComplete(const FrequencySpectrum &spectrum);

private:
	enum State { Idle, Busy, Cancelled };
	SpectrumAnalyzerThread *mThread;
	State mState;
};

#endif // SPECTRUMANALYZER_H
