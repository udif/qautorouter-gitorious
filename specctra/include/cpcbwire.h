/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBWIRE_H
#define CPCBWIRE_H

#include "cspecctraobject.h"

#include <QPointF>
#include <QPainterPath>
#include <QWidget>

class CPcbNet;
class CPcbClearanceClass;
class CPcbPolylinePath;
class CPcbWire : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbWire(QGraphicsItem *parent = 0);
		virtual ~CPcbWire();

		double						width();
		QString						layer();

		CPcbPolylinePath*			polylinePath();
		CPcbNet*					net();
		CPcbClearanceClass*			clearanceClass();

		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
};

#endif // CPCBWIRE_H

