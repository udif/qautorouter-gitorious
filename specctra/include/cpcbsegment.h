/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBSEGMENT_H
#define CPCBSEGMENT_H

#include <QObject>
#include <QGraphicsItem>
#include <QColor>

#include "cspecctraobject.h"

/**
  * @brief Baseclass for drawable wires, vias and the like.
  */

class CPcbNet;
class CPcbClearanceClass;
class CPcbSegment : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbSegment(QGraphicsItem *parent = 0);
		virtual ~CPcbSegment();

		virtual double					layerIndex()=0;
		virtual double					width()=0;
		virtual QColor					color()=0;
		virtual bool					drawable();
		virtual CPcbNet*				net();
		virtual CPcbClearanceClass*		clearanceClass();

		/** QGraphicsItem overrides... */
		virtual QRectF					boundingRect() const;
		virtual void					paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
};

#endif // CPCBSEGMENT_H

