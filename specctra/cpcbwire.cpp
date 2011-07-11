/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbwire.h"
#include "cpcbpolylinepath.h"
#include "cpcbnet.h"
#include "cpcbclearanceclass.h"
#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcblayer.h"

#define inherited CPcbSegment

CPcbWire::CPcbWire(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbWire::~CPcbWire()
{
}

bool CPcbWire::drawable()
{
	if ( width() >= 1 )
		return true;
	return false;
}

double CPcbWire::width()
{
	if ( polylinePath() != NULL )
		return polylinePath()->width();
	return 0.0;
}

QString CPcbWire::layerRef()
{
	QString rc;
	if ( polylinePath() != NULL )
		rc = polylinePath()->layer();
	return rc;
}

CPcbLayer* CPcbWire::layer()
{
	if ( pcb() != NULL && pcb()->structure() != NULL )
		return pcb()->structure()->layer(layerRef());
	return NULL;
}

QColor CPcbWire::color()
{
	QColor c(0,255,0);
	if ( layer() != NULL )
		c = layer()->color();
	return c;
}

CPcbPolylinePath* CPcbWire::polylinePath()
{
	return (CPcbPolylinePath*)child("polyline_path");
}

QPainterPath CPcbWire::shape() const
{
	CPcbWire* me=(CPcbWire*)this;
	QPainterPath ppath;
	if ( me->polylinePath() != NULL )
	{
		QPolygonF polygon;
		QPointF pt;
		for(int n=0; n < me->polylinePath()->points(); n++)
		{
			pt = me->polylinePath()->point(n);
			if ( fabs(pt.x()) > 0.1 && fabs(pt.y()) > 0.1 )
				polygon.append(pt);
		}
		ppath.addPolygon(polygon);
	}
	return ppath;
}


