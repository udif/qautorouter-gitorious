/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include <QtGui/QApplication>
#include <QString>
#include "qautorouter.h"
#include "cspecctrareader.h"
#include "cspecctraobject.h"
#include "cpcb.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Q_INIT_RESOURCE(qautorouter);
	QAutoRouter w;
	w.show();
	if ( argc == 2 )
	{
		QFile file(argv[1]);
		if ( !w.load(file) )
		{
			fprintf(stderr,"error loading file.");
			exit(-1);
		}
	}
	return a.exec();
}
