/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBPLANE_H
#define CPCBPLANE_H

#include <QObject>

#include "cpcbsegment.h"

class CPcbNet;
class CPcbPath;
class CPcbLayer;
class CPcbPlane : public CPcbSegment
{
	Q_OBJECT
	public:
		CPcbPlane(QGraphicsItem *parent = 0);
		virtual ~CPcbPlane();

		virtual double					layerIndex();
		virtual double					width();
		virtual QColor					color();
		virtual bool					drawable();
		virtual CPcbNet*				net();
		virtual QString					netRef();
		virtual QString					layerRef();
		virtual CPcbLayer*				layer();
		virtual CPcbPath*				path();

		/** QGraphicsItem overrides... */
		virtual QPainterPath			shape() const;
		virtual void					paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
};

#endif // CPCBPLANE_H

