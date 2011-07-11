/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBVIA_H
#define CPCBVIA_H

#include <QObject>
#include <QGraphicsItem>

#include "cpcbsegment.h"

class CPcbPadstack;
class CPcbVia : public CPcbSegment
{
	Q_OBJECT
	public:
		CPcbVia(QGraphicsItem *parent = 0);
		virtual ~CPcbVia();

		virtual CSpecctraObject::tDrawableClass			drawableClass();

		virtual double					layerIndex();
		virtual bool					drawable();
		virtual QPointF					center();
		virtual double					width();
		virtual QColor					color();
		virtual CPcbPadstack*			padstack();

		/** QGraphicsItem overrides... */
		virtual QPainterPath			shape() const;
		virtual void					paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
};

#endif // CPCBVIA_H

