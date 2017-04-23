TEMPLATE = app

QT += multimedia qml quick gui widgets

INCLUDEPATH += ffft

SOURCES += main.cpp \
	audioengine.cpp \
	utils.cpp \
	frequencyspectrum.cpp \
	spectrumanalyzer.cpp \
	ffftwrapper.cpp \
	spectrumdisplay.cpp

RESOURCES += \
	spectrum.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
	audioengine.h \
	utils.h \
	frequencyspectrum.h \
	spectrumanalyzer.h \
	ffft/FFTRealFixLenParam.h \
	ffft/FFTReal.h \
	ffft/FFTReal.hpp \
	ffft/Array.h \
	ffft/Array.hpp \
	ffft/def.h \
	ffft/DynArray.h \
	ffft/DynArray.hpp \
	ffft/FFTRealFixLen.h \
	ffft/FFTRealFixLen.hpp \
	ffft/FFTRealPassDirect.h \
	ffft/FFTRealPassDirect.hpp \
	ffft/FFTRealPassInverse.h \
	ffft/FFTRealPassInverse.hpp \
	ffft/FFTRealSelect.h \
	ffft/FFTRealSelect.hpp \
	ffft/FFTRealUseTrigo.h \
	ffft/FFTRealUseTrigo.hpp \
	ffft/OscSinCos.h \
	ffft/OscSinCos.hpp \
	ffftwrapper.h \
	def.h \
	spectrumdisplay.h

DISTFILES +=

FORMS +=
