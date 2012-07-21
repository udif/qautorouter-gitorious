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

		virtual CSpecctraObject::tDrawableClass			drawableClass();

		QString							unit();
		QPointF							pos();
		QString							side();
		CPcbPin*						pin(QString ref);
		int								pads();
		CGPadstack*						pad(int idx);
		CGPadstack*						pad(QString ref);
		double							rotation();
		QPainterPath&					outlineShape();
		virtual QRectF					boundingRect() const;
		virtual QPainterPath			shape() const;
		virtual void					paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);


        /* gEDA export methods */
        virtual QString					gedaObjectClass();
        virtual QStringList				gedaProperties();
        virtual QList<CSpecctraObject*>	gedaChildren();

	protected:
		void							createPadstacks();
	private:
		bool							mPadstacksCreated;
		QPainterPath					mOutlineShape;
		QList<CGPadstack*>				mPads;
};

#endif // CPCBPLACE_H

