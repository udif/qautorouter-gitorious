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
	return properties()[0];
}

double CPcbPolylinePath::width()
{
	return properties()[1].toDouble();
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
	for(int n=0; n < me->points(); n++)
	{
		QPointF pt = me->point(n);
		if (n==0)
			ppath.moveTo(pt);
		else
			ppath.lineTo(pt);
	}
	return ppath;
}

