/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBPADSTACK_H
#define CPCBPADSTACK_H

#include <QObject>
#include <QString>

#include "cspecctraobject.h"

class CPcbShape;
class CPcbPadstack : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbPadstack(QGraphicsItem *parent = 0);
		virtual ~CPcbPadstack();

		QString			name();

		int				shapes();
		CPcbShape*		shape(int idx);
};

#endif // CPCBPADSTACK_H

