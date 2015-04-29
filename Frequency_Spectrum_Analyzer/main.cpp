#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include "audioengine.h"
#include "spectrumdisplay.h"

#define NDEBUG

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	AudioEngine engine;

	QQuickView view;
	view.setFlags(Qt::FramelessWindowHint);
	view.setColor(QColor(0, 0, 0, 0));
	view.rootContext()->setContextProperty("engine", &engine);
	view.setSource(QUrl("qrc:/Main.qml"));

	SpectrumDisplay frequencyBands(view.engine(), view.rootObject());
	// send spectrum analysis results to the UI handler
	QObject::connect(&engine, SIGNAL(spectrumChanged(const FrequencySpectrum&)),
					 &frequencyBands, SLOT(updateBands(const FrequencySpectrum&)));

	view.setPosition(2200, 600);
	view.show();
	engine.startRecording();

	return app.exec();
}
