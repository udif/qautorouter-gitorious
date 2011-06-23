/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBPLACE_H
#define CPCBPLACE_H

#include <QObject>
#include <QString>
#include <QPolygonF>
#include <QMap>

#include "cspecctraobject.h"
#include "cgpadstack.h"

class CPcbPin;
class CPcbPlace : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbPlace(QGraphicsItem *parent = 0);
		virtual ~CPcbPlace();

		QString							unit();
		QPointF							pos();
		QString							side();
		CPcbPin*						pin(QString index);
		double							rotation();
		QPainterPath					outlineShape() const;
		virtual QRectF					boundingRect() const;
		virtual QPainterPath			shape() const;
		virtual void					paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	protected:
		void							createPadstacks();
	private:
		bool							mPadstacksCreated;
		QPainterPath					mOutlineShape;
		QList<CGPadstack*>				mPads;
};

#endif // CPCBPLACE_H

