# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
QT += gui
win32: CONFIG += qt plugin
unix: CONFIG += dll plugin debug
INCLUDEPATH += ../../include ../../graphics/include ../../specctra/include
TARGET = simpleplugin
TEMPLATE = lib
unix {
	target.path = /opt/qautorouter/plugins
	INSTALLS += target
}
SOURCES += simplerouter.cpp
HEADERS += simplerouter.h
OTHER_FILES += README.txt
LIBS += -L../../graphics -L../../specctra -lqagraphics -lqaspecctra

