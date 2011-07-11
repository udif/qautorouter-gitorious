/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBWIRE_H
#define CPCBWIRE_H

#include "cpcbsegment.h"

#include <QPointF>
#include <QPainterPath>
#include <QWidget>

class CPcbNet;
class CPcbClearanceClass;
class CPcbPath;
class CPcbLayer;
class CPcbWire : public CPcbSegment
{
	Q_OBJECT
	public:
		CPcbWire(QGraphicsItem *parent = 0);
		virtual ~CPcbWire();

		virtual CSpecctraObject::tDrawableClass			drawableClass();

		virtual double					layerIndex();
		virtual bool					drawable();
		virtual double					width();
		virtual QColor					color();
		virtual QString					layerRef();
		virtual CPcbLayer*				layer();
		virtual CPcbPath*				path();

		/** QGraphicsItem overrides... */
		virtual QPainterPath			shape() const;
};

#endif // CPCBWIRE_H

