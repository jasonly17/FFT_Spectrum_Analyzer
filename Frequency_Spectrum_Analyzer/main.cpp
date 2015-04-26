#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

#include "audioengine.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	AudioEngine engine;

	QQuickView view;
	view.rootContext()->setContextProperty("engine", &engine);
	view.setSource(QUrl("qrc:/Main.qml"));
	view.show();

	engine.startRecording();

	return app.exec();
}
