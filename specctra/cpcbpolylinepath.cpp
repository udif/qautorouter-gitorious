/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbpolylinepath.h"

#define inherited CSpecctraObject

CPcbPolylinePath::CPcbPolylinePath(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbPolylinePath::~CPcbPolylinePath()
{
}

QString CPcbPolylinePath::layer()
{
	QString rc;
	if ( properties().count() )
		rc = properties()[0];
	return rc;
}

double CPcbPolylinePath::width()
{
	double w=0.0;
	if ( properties().count() >= 2 )
		w = properties()[1].toDouble();
	return w;
}

int CPcbPolylinePath::coords()
{
	return properties().count()-2;
}

double CPcbPolylinePath::coord(int idx)
{
	return properties()[idx+2].toDouble();
}

int CPcbPolylinePath::points()
{
	return coords()/2;
}

QPointF CPcbPolylinePath::point(int idx)
{
	int x = (idx*2)+2;
	int y = (idx*2)+3;
	QPointF pt(properties()[x].toDouble(),properties()[y].toDouble());
	return pt;
}

QRectF CPcbPolylinePath::boundingRect() const
{
	return shape().boundingRect();
}

QPainterPath CPcbPolylinePath::shape() const
{
	CPcbPolylinePath* me=(CPcbPolylinePath*)this;
	QPainterPath ppath;
	QPointF pt;
	for(int n=0; n < me->coords(); n+=2)
	{
		int x = me->coord(n);
		int y = me->coord(n+1);
		if (n==0)
		{
			pt = QPointF(x,y);
			ppath.moveTo(pt);
		}
		else
		{
			if ( fabs(x) <= 0.1 ) x = pt.x();
			if ( fabs(y) <= 0.1 ) y = pt.y();
			pt = QPointF(x,y);
			ppath.lineTo(pt);
		}
	}
	return ppath;
}

