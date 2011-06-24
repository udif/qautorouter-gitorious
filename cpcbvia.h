/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBVIA_H
#define CPCBVIA_H

#include <QObject>
#include <QString>
#include <QRectF>
#include <QPainterPath>

#include "cspecctraobject.h"

class CPcbPadstack;
class CPcbVia : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbVia(QGraphicsItem *parent = 0);
		virtual ~CPcbVia();

		CPcbPadstack*					padstack();

};

#endif // CPCBVIA_H

