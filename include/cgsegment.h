/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGSEGMENT_H
#define CGSEGMENT_H

#include <QObject>
#include <QPainterPath>
#include <QGraphicsItem>

class CGSegment : public QObject, public QGraphicsItem
{
	Q_OBJECT
	public:
		CGSegment(QObject *parent = 0);
		virtual ~CGSegment();

		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

};

#endif // CGSEGMENT_H
