#include "spectrumdisplay.h"
#include <QDebug>
#include <QtMath>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include "def.h"
#include "frequencyspectrum.h"

/******************************************************************************
 * SpectrumDisplay
******************************************************************************/
SpectrumDisplay::SpectrumDisplay(QQmlEngine *engine, QQuickItem *root, QObject *parent)
	: QObject(parent)
	, engine(engine)
	, root(root)
	, updating(false)
{
	// create the bands and store them for future reference
    root->findChild<QQuickItem*>("divider")->setWidth(BANDS * BANDWIDTH + (BANDS - 1) * 2);
	QQuickItem *container = root->findChild<QQuickItem*>("bands");
	QQuickItem *containerReflection = root->findChild<QQuickItem*>("bandsReflection");

	QQmlComponent component(engine, QUrl("qrc:/SpectrumBand.qml"));
	for (int i = 0; i < BANDS; i++) {
		QQuickItem *band = qobject_cast<QQuickItem*>(component.create());
		band->setHeight(BANDHEIGHT);
		band->setWidth(BANDWIDTH);
		band->setParentItem(container);
		engine->setObjectOwnership(band, QQmlEngine::CppOwnership);
		bands.append(band);
		//bandHeights.append(1.0f);

		if (REFLECTION) {
			band = qobject_cast<QQuickItem*>(component.create());
			band->setHeight(BANDHEIGHT);
			band->setWidth(BANDWIDTH);
			band->setOpacity(0.5);
			band->setParentItem(containerReflection);
			band->setProperty("anchorBottom", "false");
			engine->setObjectOwnership(band, QQmlEngine::CppOwnership);
			bandsReflection.append(band);
		}
	}

	// precalculate the bin ranges for use when displaying the frequency bands
	if (!MIRROR) {
		//int lastIndex = -1;
		float freqRange = log10(2000) - 1;
		float binStep = freqRange / BANDS;
		qDebug() << freqRange << binStep;
		for (int i = 0; i < BANDS; i++) {
			//float freq = qExp((1 + i * binStep) * qLn(10));
			//qDebug() << "freq:" << freq;
			//int index = qRound(freq / (SAMPLERATE / 2.0f / SAMPLES));
			//int index = qPow(2, i * 10.0 / (BANDS - 1)) * 1.52 - 1;
			//int index = qPow(2, i * 10.0 / (BANDS - 1)) * 3 - 1 + 80;
			int index = i;

			//int index = lastIndex + i / 200.0f;
			//lastIndex = index + 1;

//			int index = i * (SAMPLES / (BANDS + 1.0f)) * i / (BANDS - 1.0f);
//			if (index <= lastIndex) index = lastIndex + 1;
//			lastIndex = index;

//			int index = qPow(1.014f, (i - 4) * 3) - 1;
//			if (index <= lastIndex) index = lastIndex + 1;
//			lastIndex = index;
			//if (index >= SAMPLES / 2) index = SAMPLES / 2 - 1;
			if (index >= SAMPLES) index = SAMPLES - 1;
			//if (index >= 16000.0f / (SAMPLERATE / 2.0f / SAMPLES)) index = 16000.0f / (SAMPLERATE / 2.0f / SAMPLES) - 1;
			binIndices.append(index);
			//qDebug() << i << index;
		}
//		for (int i = 0; i < BANDS; i++) {
//			int index = qPow(2, (i + 23) * 8.0 / (BANDS - 1)) * 1.52 - 1;
//			if (index >= SAMPLES) index = SAMPLES - 1;
//			binIndices.append(index + 2);
//			qDebug() << index;
//		}
	} else {
		for (int i = 0; i < BANDS / 2; i++) {
			int index = qPow(2, i * 2.0 * 10.0 / (BANDS - 1)) * 1.52 - 1;
			if (index >= SAMPLES / 2) index = SAMPLES / 2 - 1;
			if (index >= SAMPLES) index = SAMPLES - 1;
			binIndices.append(index + 2);
			//qDebug() << index;
		}
	}
}

SpectrumDisplay::~SpectrumDisplay()
{
	foreach (QQuickItem *band, bands) band->deleteLater();
}

/*********** Public Slots ***********/
// Updates the bands with the data provided by spectrum.
void SpectrumDisplay::updateBands(const FrequencySpectrum &spectrum)
{
	if (updating) {
		qDebug() << "updating in progress";
		return;
	}

	updating = true;
	int lastIndex = 0;
	//float last = 0.0f;
	if (!MIRROR) {
//		for (int i = 0; i < BANDS; i++) {
//			float peak = 0;
//			int loIndex = binIndices[i];
//			if (index <= lastIndex) index = lastIndex + 2;

//			for (; lastIndex < index; lastIndex++) {

//			}
//		}
		for (int i = 0; i < BANDS; i++) {
			float peak = 0.0f;
			int index = binIndices[i];
			if (index <= lastIndex) index = lastIndex + 1;

			for (; lastIndex < index; lastIndex++) {
				if (peak < spectrum[lastIndex].amplitude) {
					peak = spectrum[lastIndex].amplitude;
				}
			}

			if (peak < 0.25) peak *= 0.2f;
			else if (peak < 0.325) peak *= 0.25f;
			else if (peak < 0.4) peak *= 0.4f;
			else if (peak < 0.55) peak *= 0.7f;
			else if (peak < 0.625) peak *= 0.775f;
			else if (peak < 0.7) peak *= 0.95f;
			else if (peak < 0.85) peak *= 1.0f;
			else peak *= 1.05f;

//			if (peak < 0.25) peak *= 0.65f;
//			else if (peak < 0.4) peak *= 0.7f;
//			else if (peak < 0.55) peak *= 0.9f;
//			else if (peak < 0.7) peak *= 0.95f;
//			else peak *= 1.05f;

			// max index = 4096
			//if (index > 171) peak *= 0.5;
			if (index > 86) peak *= 0.6;
			else if (index > 43) peak *= 0.7;
			else if (index > 21) peak *= 0.85;

			//peak = peak * 0.8f + last * 0.2f;

			//if (i == 0 || i == 76 || i == 152 || i == 228) bands[i]->setProperty("bandHeight", 500);
			//else if (i == 150) bands[i]->setProperty("bandHeight", 500);
			bands[i]->setProperty("bandHeight", peak * BANDHEIGHT);
			if (REFLECTION) bandsReflection[i]->setProperty("bandHeight", peak * BANDHEIGHT / 2);
			//bandHeights[i] = peak * 200;
			//last = peak;
		}
	} else {
		for (int i = 0; i < BANDS / 2; i++) {
			float peak = 0;
			int index = binIndices[i];
			if (index <= lastIndex) index = lastIndex + 1;

			for (; lastIndex < index; lastIndex++) {
				if (peak < spectrum[lastIndex].amplitude) {
					peak = spectrum[lastIndex].amplitude;
				}
			}

			// makes the louder frequency bands more pronounced
			if (peak < 0.25) peak *= 0.3f;
			else if (peak < 0.4) peak *= 0.6f;
			else if (peak < 0.55) peak *= 0.8f;
			else if (peak < 0.7) peak *= 1.1f;
			else if (peak < 0.85) peak *= 1.5f;

			bands[i]->setProperty("bandHeight", peak * BANDHEIGHT);
			bands[BANDS - i - 1]->setProperty("bandHeight", peak * BANDHEIGHT);
			if (REFLECTION) {
				bandsReflection[i]->setProperty("bandHeight", peak * BANDHEIGHT);
				bandsReflection[BANDS - i - 1]->setProperty("bandHeight", peak * BANDHEIGHT / 2);
			}
		}
	}
	updating = false;
}
