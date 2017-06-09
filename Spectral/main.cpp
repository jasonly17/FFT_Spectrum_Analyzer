#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtCore/QFile>

#include "AudioEngine.h"

int main(int argc, char *argv[])
{
	QGuiApplication a(argc, argv);

	AudioEngine audioEngine;
	audioEngine.start();

	QQuickView view;
	
	view.setPosition(600, 1100);
	view.show();

	return a.exec();
}
