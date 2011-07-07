/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGPAD_H
#define CGPAD_H

#include <QObject>
#include <QPainterPath>
#include <QString>
#include <QGraphicsItem>

class CPcb;
class CGPadstack;
class CPcbLayer;
class CGPad : public QObject, public QGraphicsItem
{
	Q_OBJECT
	public:
		explicit CGPad(CGPadstack* padstack, QPainterPath shape, QString layer, QObject *parent = 0);

		CGPadstack*					padstack() {return mPadstack;}
		CPcb*						pcb();
		QString&					layer() {return mLayer;}
		QString						pinRef();
		QPointF						pos();
		QColor						color();
		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

	private:
		CGPadstack*					mPadstack;
		QPainterPath				mShape;
		QString						mLayer;
		QRectF						mBoundingRect;
		CPcbLayer*					mLayerObject;
};

#endif // CGPAD_H
