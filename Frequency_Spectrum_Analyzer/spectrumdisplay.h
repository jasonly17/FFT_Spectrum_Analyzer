#ifndef SPECTRUMDISPLAY_H
#define SPECTRUMDISPLAY_H

#include <QObject>

class FrequencySpectrum;
class QQmlEngine;
class QQuickItem;

/******************************************************************************
 * Handles the creation and updating of the frequency bands from the audio data
 * processed by the spectrum analyzer.
******************************************************************************/

class SpectrumDisplay : public QObject
{
	Q_OBJECT
public:
	explicit SpectrumDisplay(QQmlEngine *engine, QQuickItem *root, QObject *parent = 0);
	~SpectrumDisplay();

public slots:
	void updateBands(const FrequencySpectrum &spectrum);

private:
	QQmlEngine *engine;
	QQuickItem *root;
	QList<QQuickItem*> bands;
	QList<QQuickItem*> bandsReflection;
	//QList<int> bandHeights;
	QList<float> binIndices;
	bool updating;
};

#endif // SPECTRUMDISPLAY_H
