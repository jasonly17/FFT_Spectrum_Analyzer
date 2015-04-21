TEMPLATE = app

QT += qml quick widgets multimedia

SOURCES += main.cpp \
	audioengine.cpp \
#    audiobuffer.cpp \
#    ringbuffer.cpp \
#    utils.cpp \
#    fftreal/stopwatch/ClockCycleCounter.cpp \
#    fftreal/stopwatch/StopWatch.cpp \
#    fftreal/fftreal_wrapper.cpp \
#    frequencyspectrum.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
	audioengine.h \
#    audiobuffer.h \
#    ringbuffer.h \
#	utils.h \
#    fftreal/stopwatch/ClockCycleCounter.h \
#    fftreal/stopwatch/ClockCycleCounter.hpp \
#    fftreal/stopwatch/def.h \
#    fftreal/stopwatch/fnc.h \
#    fftreal/stopwatch/fnc.hpp \
#    fftreal/stopwatch/Int64.h \
#    fftreal/stopwatch/StopWatch.h \
#    fftreal/stopwatch/StopWatch.hpp \
#    fftreal/Array.h \
#    fftreal/Array.hpp \
#    fftreal/def.h \
#    fftreal/DynArray.h \
#    fftreal/DynArray.hpp \
#    fftreal/FFTReal.h \
#    fftreal/FFTReal.hpp \
#    fftreal/fftreal_wrapper.h \
#    fftreal/FFTRealFixLen.h \
#    fftreal/FFTRealFixLen.hpp \
#    fftreal/FFTRealFixLenParam.h \
#    fftreal/FFTRealPassDirect.h \
#    fftreal/FFTRealPassDirect.hpp \
#    fftreal/FFTRealPassInverse.h \
#    fftreal/FFTRealPassInverse.hpp \
#    fftreal/FFTRealSelect.h \
#    fftreal/FFTRealSelect.hpp \
#    fftreal/FFTRealUseTrigo.h \
#    fftreal/FFTRealUseTrigo.hpp \
#    fftreal/OscSinCos.h \
#    fftreal/OscSinCos.hpp \
#    frequencyspectrum.h
