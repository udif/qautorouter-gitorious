/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbcircle.h"
#include <QPointF>
#include <math.h>

#define inherited CSpecctraObject

CPcbCircle::CPcbCircle(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbCircle::~CPcbCircle()
{
}

double CPcbCircle::radius()
{
	return properties().at(1).toDouble()/2.0;
}

QString CPcbCircle::layer()
{
	return properties().at(0);
}

QPainterPath CPcbCircle::path()
{
	QPainterPath ppath;
	QPoint topLeft(-radius(),-radius());
	QPoint bottomRight(radius(),radius());
	ppath.addEllipse(QRectF(topLeft,bottomRight));
	return ppath.simplified();
}
