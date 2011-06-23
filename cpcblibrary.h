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
class CPcbLibrary : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbLibrary(QGraphicsItem *parent = 0);
		virtual ~CPcbLibrary();

		int								images();
		CPcbImage*						image(int comp);
		CPcbImage*						image(QString n);
};

#endif // CPCBLIBRARY_H

