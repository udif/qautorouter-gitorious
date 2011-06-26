/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgsegment.h"

#include <QPainter>

#define inherited QObject

CGSegment::CGSegment(QObject *parent)
: inherited(parent)
{
}

CGSegment::~CGSegment()
{
}

QRectF CGSegment::boundingRect() const
{
	QRectF bounds = shape().boundingRect();
	return bounds;
}

QPainterPath CGSegment::shape() const
{
	QPainterPath ppath;
	return ppath;
}

void CGSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPainterPath p = shape();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->scale(scale(),scale());
	painter->setPen(QPen(QColor(255, 255, 255), 3, Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
	painter->drawPath(shape());
}
