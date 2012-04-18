# -------------------------------------------------
# Project created by QtCreator 2011-06-16T09:55:28
# -------------------------------------------------
#QT += gui
win32: CONFIG += staticlib
unix: CONFIG += staticlib
INCLUDEPATH += /usr/include/glib-2.0 -/usr/lib/i386-linux-gnu/glib-2.0/include /usr/lib/i386-linux-gnu/glib-2.0/include/
TARGET = gts
TEMPLATE = lib
unix {
	target.path = .
	INSTALLS += target
}
SOURCES += bbtree.c boolean.c cdt.c container.c curvature.c edge.c eheap.c face.c fifo.c graph.c heap.c hsurface.c iso.c isotetra.c kdtree.c matrix.c misc.c named.c object.c oocs.c partition.c pgraph.c point.c predicates.c psurface.c refine.c segment.c split.c stripe.c surface.c triangle.c tribox3.c vertex.c vopt.c
HEADERS += gts.h gts_lists.h gts-private.h predicates.h rounding.h
OTHER_FILES += 
unix: LIBS += -lglib-2.0
win32: LIBS += -L../../../qautorouter-build-desktop/specctra/release -L../../../qautorouter-build-desktop/specctra/debug -lspecctra

