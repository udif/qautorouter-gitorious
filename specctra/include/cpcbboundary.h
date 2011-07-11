/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBOUNDARY_H
#define CPCBOUNDARY_H


#include "cspecctraobject.h"

#include <QObject>
#include <QString>

class CPcbPath;
class CPcbBoundary : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbBoundary(QGraphicsItem *parent = 0);
		virtual ~CPcbBoundary();

		virtual CSpecctraObject::tDrawableClass			drawableClass();

		CPcbPath*					path();
		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
};

#endif // CPCBLAY_H

