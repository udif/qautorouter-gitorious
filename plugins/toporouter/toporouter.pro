# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
QT += gui
DEFINES+='COORD_TYPE="long long"'
DEFINES+='COORD_MAX="LLONG_MAX"'
win32: CONFIG += qt plugin
unix: CONFIG += dll plugin debug -std=c99
QMAKE_CFLAGS += -std=c99 -Wl,--no-undefined
QMAKE_CXXFLAGS += -std=c99 -Wl,--no-undefined
INCLUDEPATH += ../../include ../../graphics/include ../../specctra/include gts /usr/include/glib-2.0 /usr/lib/x86_64-linux-gnu/glib-2.0/include /usr/lib/glib-2.0/include/
TARGET = toporouter
TEMPLATE = lib
unix {
	target.path = /opt/qautorouter/plugins
	INSTALLS += target
}
SOURCES += topoplugin.cpp
SOURCES += toporouter.c
SOURCES += data.c

HEADERS += toporouter.h
HEADERS += topoplugin.h

OTHER_FILES += README.txt
unix: LIBS += -L../../specctra -lspecctra gts/libgts.a
win32: LIBS +=  -L../../../qautorouter-build-desktop/specctra/release -L../../../qautorouter-build-desktop/specctra/debug -lspecctra
SUBDIRS = gts
