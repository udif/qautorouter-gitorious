/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbrect.h"

#define inherited CSpecctraObject

CPcbRect::CPcbRect(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbRect::~CPcbRect()
{
}

QRectF CPcbRect::rect()
{
	QRectF r(QPointF(properties().at(1).toDouble(),properties().at(2).toDouble()),QPointF(properties().at(3).toDouble(),properties().at(4).toDouble()));
	return r;
}

QString CPcbRect::layer()
{
	return properties().at(0);
}

/**
  * @return a QPainterPath to draw the shape
  */
QPainterPath CPcbRect::path()
{
	QRectF r = rect();
	QPainterPath ppath;
	ppath.addRect(r);
	return ppath;
}
