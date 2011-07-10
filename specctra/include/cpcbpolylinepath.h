/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBPOLYLINEPATH_H
#define CPCBPOLYLINEPATH_H

#include "cspecctraobject.h"

#include <QPointF>
#include <QString>
#include <QRectF>
#include <QPainterPath>

class CPcbPolylinePath : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbPolylinePath(QGraphicsItem *parent = 0);
		virtual ~CPcbPolylinePath();

		QString							layer();
		double							width();

		int								coords();
		double							coord(int idx);

		int								points();
		QPointF							point(int idx);

		virtual QRectF					boundingRect() const;
		virtual QPainterPath			shape() const;

};

#endif // CPCBPOLYLINEPATH_H

