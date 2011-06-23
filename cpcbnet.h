/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBNET_H
#define CPCBNET_H

#include <QObject>
#include <QString>

#include "cspecctraobject.h"
#include "cgpadstack.h"

class CPcbPins;
class CPcbNet : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbNet(QGraphicsItem *parent = 0);
		virtual ~CPcbNet();

		QString						name();

		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	private:
		QPainterPath				mShape;
};

#endif // CPCBNET_H

