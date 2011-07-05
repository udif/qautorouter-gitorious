# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
QT += gui
win32: CONFIG += qt plugin
unix: CONFIG += dll plugin debug
INCLUDEPATH += ../../include
TARGET = simpleplugin
TEMPLATE = lib
SOURCES += simplerouter.cpp
HEADERS += simplerouter.h
OTHER_FILES += README.txt
