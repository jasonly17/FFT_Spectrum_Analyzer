#include "spectrumdisplay.h"
#include <QDebug>
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
{
	// create the bands and store them for future reference
	root->findChild<QQuickItem*>("divider")->setWidth(BANDS * 8 + (BANDS - 1) * 4);
	QQuickItem *container = root->findChild<QQuickItem*>("bands");
	QQuickItem *containerReflection = root->findChild<QQuickItem*>("bandsReflection");
	QQmlComponent component(engine, QUrl("qrc:/SpectrumBand.qml"));
	for (int i = 0; i < BANDS; i++) {
		QQuickItem *band = qobject_cast<QQuickItem*>(component.create());
		band->setHeight(200);
		band->setWidth(8);
		band->setParentItem(container);
		engine->setObjectOwnership(band, QQmlEngine::CppOwnership);
		bands.append(band);

		if (REFLECTION) {
			band = qobject_cast<QQuickItem*>(component.create());
			band->setHeight(200);
			band->setWidth(8);
			band->setOpacity(0.5);
			band->setParentItem(containerReflection);
			band->setProperty("anchorBottom", "false");
			engine->setObjectOwnership(band, QQmlEngine::CppOwnership);
			bandsReflection.append(band);
		}
	}

	// precalculate the bin ranges for use when displaying the frequency bands
	if (!MIRROR) {
		for (int i = 0; i < BANDS; i++) {
			int index = pow(2, i * 10.0 / (BANDS - 1)) * 1.52 - 1;
			if (index >= SAMPLES / 2) index = SAMPLES / 2 - 1;
			binIndices.append(index);
			qDebug() << index;
		}
	} else {
		for (int i = 0; i < BANDS / 2; i++) {
			int index = pow(2, i * 2.0 * 10.0 / (BANDS - 1)) * 1.52 - 1;
			if (index >= SAMPLES / 2) index = SAMPLES / 2 - 1;
			binIndices.append(index);
			qDebug() << index;
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
	int lastIndex = 0;
	float lastPeak = 0;
	if (!MIRROR) {
		for (int i = 0; i < BANDS; i++) {
			float peak = 0;
			int index = binIndices[i];
			if (index <= lastIndex) index = lastIndex + 1;

			for (; lastIndex < index; lastIndex++) {
				if (peak < spectrum[lastIndex].amplitude) {
					peak = spectrum[lastIndex].amplitude;
				}
			}

			// makes the louder frequency bands more pronounced
			float basePeak = peak;
			if (peak < 0.25) peak *= 0.3f;
			else if (peak < 0.34) peak *= 0.6f;
			else if (peak < 0.5) peak *= 0.8f;
			else if (peak < 0.75) peak *= 0.9f;

			bands[i]->setProperty("bandHeight", peak * 200);
			if (REFLECTION) bandsReflection[i]->setProperty("bandHeight", peak * 100);
			lastPeak = basePeak;
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
			float basePeak = peak;
			if (peak < 0.25) peak *= 0.3f;
			else if (peak < 0.34) peak *= 0.6f;
			else if (peak < 0.5) peak *= 0.8f;
			else if (peak < 0.75) peak *= 0.9f;

			bands[i]->setProperty("bandHeight", peak * 200);
			bands[BANDS - i - 1]->setProperty("bandHeight", peak * 200);
			if (REFLECTION) {
				bandsReflection[i]->setProperty("bandHeight", peak * 200);
				bandsReflection[BANDS - i - 1]->setProperty("bandHeight", peak * 100);
			}
			lastPeak = basePeak;
		}
	}
}
