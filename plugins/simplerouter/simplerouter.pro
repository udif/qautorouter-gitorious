# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
QT += gui
win32: CONFIG += qt plugin
unix: CONFIG += dll plugin debug
INCLUDEPATH += ../../include ../../graphics/include ../../specctra/include
TARGET = simplerouter
TEMPLATE = lib
unix {
	target.path = /opt/qautorouter/plugins
	INSTALLS += target
}
SOURCES += simplerouter.cpp \
    boundingbox.cpp
HEADERS += simplerouter.h \
    boundingbox.h
OTHER_FILES += README.txt
unix: LIBS += -L../../specctra -lspecctra
win32: LIBS += -L../../../qautorouter-build-desktop/specctra/release -L../../../qautorouter-build-desktop/specctra/debug -lspecctra
