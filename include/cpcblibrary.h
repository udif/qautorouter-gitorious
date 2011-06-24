/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBLIBRARY_H
#define CPCBLIBRARY_H

#include <QObject>
#include <QString>

#include "cspecctraobject.h"

class CPcbImage;
class CPcbPadstack;
class CPcbLibrary : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbLibrary(QGraphicsItem *parent = 0);
		virtual ~CPcbLibrary();

		int								images();
		CPcbImage*						image(int idx);
		CPcbImage*						image(QString ref);

		int								padstacks();
		CPcbPadstack*					padstack(int idx);
		CPcbPadstack*					padstack(QString ref);
};

#endif // CPCBLIBRARY_H

