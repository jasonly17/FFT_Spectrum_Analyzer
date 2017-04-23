#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QScreen>
#include "audioengine.h"
#include "spectrumdisplay.h"
#include "def.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AudioEngine engine;

    QQuickView view;
    view.setFlags(Qt::FramelessWindowHint);
    view.setColor(QColor(0, 0, 0, 0));
    view.rootContext()->setContextProperty("engine", &engine);
    view.setSource(QUrl("qrc:/Main.qml"));
    view.rootObject()->setProperty("appHeight", BANDHEIGHT * 2);

    SpectrumDisplay frequencyBands(view.engine(), view.rootObject());
    // send spectrum analysis results to the UI handler
    QObject::connect(&engine, SIGNAL(spectrumChanged(const FrequencySpectrum&)),
                     &frequencyBands, SLOT(updateBands(const FrequencySpectrum&)));

    view.setPosition(600, 1100);
//    view.setPosition(2320, 540);
//	view.setPosition(300, 140);
    view.show();
    engine.startRecording();

    return app.exec();
}
