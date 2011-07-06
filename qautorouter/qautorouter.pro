# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
QT += gui
#QMAKE_LFLAGS += -rdynamic
unix {
	QMAKE_LFLAGS += -rdynamic -pg
	QMAKE_CXXFLAGS_DEBUG += -pg
	QMAKE_LFLAGS_DEBUG += -pg
}
INCLUDEPATH += include ../graphics/include ../specctra/include
TARGET = qautorouter
TEMPLATE = app
unix {
	target.path = /usr/bin
	INSTALLS += target
}
SUBDIRS = graphics \
		specctra
SOURCES += main.cpp \
		cspecctrareader.cpp \
		qautorouter.cpp
HEADERS += include/qautorouter.h \
	include/cspecctrareader.h
FORMS += forms/qautorouter.ui \
    forms/preferences.ui \
    forms/layerpreferences.ui
RESOURCES += qautorouter.qrc
LIBS += -Lgraphics -Lspecctra -lqagraphics -lqaspecctra
