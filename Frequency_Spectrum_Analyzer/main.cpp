#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>

#include "audioengine.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	AudioEngine engine;
	engine.startRecording();

	return app.exec();
}
