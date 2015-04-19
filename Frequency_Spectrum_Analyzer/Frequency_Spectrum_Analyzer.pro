TEMPLATE = app

QT += qml quick widgets multimedia

SOURCES += main.cpp \
  #  audioengine.cpp \
    circularbuffer.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
   # audioengine.h \
    circularbuffer.h
