# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
QT += gui
win32: CONFIG += qt plugin
unix: CONFIG += dll plugin debug -std=c99
INCLUDEPATH += ../../include ../../graphics/include ../../specctra/include gts /usr/include/glib-2.0 -/usr/lib/i386-linux-gnu/glib-2.0/include /usr/lib/i386-linux-gnu/glib-2.0/include/
TARGET = toporouter
TEMPLATE = lib
unix {
	target.path = /opt/qautorouter/plugins
	INSTALLS += target
}
SOURCES += toporouter.cpp topoplugin.cpp
HEADERS += toporouter.h topoplugin.h
OTHER_FILES += README.txt
unix: LIBS += -L../../specctra -lspecctra gts/libgts.a
win32: LIBS +=  -L../../../qautorouter-build-desktop/specctra/release -L../../../qautorouter-build-desktop/specctra/debug -lspecctra

