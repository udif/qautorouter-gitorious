# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
QT += gui
unix: QMAKE_LFLAGS += -rdynamic
INCLUDEPATH += include
TARGET = qautorouter
TEMPLATE = app
SOURCES += main.cpp \
    qautorouter.cpp \
    cspecctrareader.cpp \
    cspecctraobject.cpp \
    cpcbstructure.cpp \
    cpcblayer.cpp \
    cpcbboundary.cpp \
    cpcb.cpp \
    cpcbplacement.cpp \
    cpcbcomponent.cpp \
    cpcbplace.cpp \
    cpcblibrary.cpp \
    cpcbimage.cpp \
    cpcbpin.cpp \
    cpcboutline.cpp \
    cpcbpath.cpp \
    cpcbpadstack.cpp \
    cpcbshape.cpp \
    cpcbcircle.cpp \
    cpcbrect.cpp \
    cpcbnetwork.cpp \
    cpcbnet.cpp \
    cpcbpins.cpp \
    cpcbclass.cpp \
    cgpad.cpp \
    cgpadstack.cpp \
    cpcbrule.cpp \
    cpcbvia.cpp \
    cgvia.cpp \
    cgsegment.cpp \
    cgwire.cpp \
    cutil.cpp
HEADERS += include/qautorouter.h \
    include/cspecctrareader.h \
    include/cspecctraobject.h \
    include/cpcbstructure.h \
    include/cpcblayer.h \
    include/cpcbboundary.h \
    include/cpcb.h \
    include/cpcbplacement.h \
    include/cpcbcomponent.h \
    include/cpcbplace.h \
    include/cpcblibrary.h \
    include/cpcbimage.h \
    include/cpcbpin.h \
    include/cpcboutline.h \
    include/cpcbpath.h \
    include/cpcbpadstack.h \
    include/cpcbpadstack.h \
    include/cpcbshape.h \
    include/cpcbcircle.h \
    include/cpcbrect.h \
    include/cpcbnetwork.h \
    include/cpcbnet.h \
    include/cpcbpins.h \
    include/cpcbclass.h \
    include/cgpad.h \
    include/cgpadstack.h \
    include/cpcbrule.h \
    include/cpcbvia.h \
    include/cplugininterface.h \
    include/cgvia.h \
    include/cgsegment.h \
    include/cgwire.h \
	include/cutil.h
FORMS += forms/qautorouter.ui \
    forms/preferences.ui \
    forms/layerpreferences.ui
RESOURCES += qautorouter.qrc
