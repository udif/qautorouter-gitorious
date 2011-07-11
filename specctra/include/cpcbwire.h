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
class CPcbPolylinePath;
class CPcbLayer;
class CPcbWire : public CPcbSegment
{
	Q_OBJECT
	public:
		CPcbWire(QGraphicsItem *parent = 0);
		virtual ~CPcbWire();

		virtual bool				drawable();
		virtual double				width();
		virtual QColor				color();
		virtual QString				layerRef();
		virtual CPcbLayer*			layer();

		virtual CPcbPolylinePath*	polylinePath();

		/** QGraphicsItem overrides... */
		virtual QPainterPath		shape() const;
};

#endif // CPCBWIRE_H

