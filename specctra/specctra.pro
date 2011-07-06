# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
QT += gui
win32: CONFIG += qt dll debug
unix: CONFIG += dll debug
INCLUDEPATH += include ../qautorouter/include ../graphics/include
TARGET = qaspecctra
TEMPLATE = lib
unix {
	target.path = /usr/lib
	INSTALLS += target
}
SOURCES += cpcb.cpp \
		cpcbboundary.cpp \
		cpcbcircle.cpp \
		cpcbclass.cpp \
		cpcbcomponent.cpp \
		cpcbimage.cpp \
		cpcblayer.cpp \
		cpcblibrary.cpp \
		cpcbnet.cpp \
		cpcbnetwork.cpp \
		cpcboutline.cpp \
		cpcbpadstack.cpp \
		cpcbpath.cpp \
		cpcbpin.cpp \
		cpcbpins.cpp \
		cpcbplace.cpp \
		cpcbplacement.cpp \
		cpcbrect.cpp \
		cpcbrule.cpp \
		cpcbshape.cpp \
		cpcbstructure.cpp \
		cpcbvia.cpp \
		cspecctraobject.cpp

HEADERS += include/cpcb.h \
		include/cpcbboundary.h \
		include/cpcbcircle.h \
		include/cpcbclass.h \
		include/cpcbcomponent.h \
		include/cpcbimage.h \
		include/cpcblayer.h \
		include/cpcblibrary.h \
		include/cpcbnet.h \
		include/cpcbnetwork.h \
		include/cpcboutline.h \
		include/cpcbpadstack.h \
		include/cpcbpath.h \
		include/cpcbpin.h \
		include/cpcbpins.h \
		include/cpcbplace.h \
		include/cpcbplacement.h \
		include/cpcbrect.h \
		include/cpcbrule.h \
		include/cpcbshape.h \
		include/cpcbstructure.h \
		include/cpcbvia.h \
		include/cplugininterface.h \
		include/cspecctraobject.h \
		include/specctra.h

OTHER_FILES += README.txt
