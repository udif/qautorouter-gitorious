/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgsegment.h"

#include <QPainter>

#define inherited QObject

CGSegment::CGSegment(QObject *parent)
: inherited(parent)
, mWidth(0.0)
, mLayer(NULL)
{
}

CGSegment::~CGSegment()
{
}

/**
  * @brief set the segment width.
  */
void CGSegment::setWidth(double w)
{
	mWidth=w;
}

/**
  * @return the segment width.
  */
double CGSegment::width()
{
	return mWidth;
}

void CGSegment::setLayer(CPcbLayer* layer)
{
	mLayer=layer;
}

CPcbLayer* CGSegment::layer()
{
	return mLayer;
}

/**
  * @brief Append a segment to the list of segments
  */
void CGSegment::append(CGSegment* segment)
{
	mSegments.append(segment);
}

/**
  * @brief get the number of segments attached to this segment.
  */
int CGSegment::segments()
{
	return mSegments.count();
}

/**
  * @return A segment by index
  */
CGSegment* CGSegment::segment(int idx)
{
	return mSegments.at(idx);
}

/**
  * @return A union of all attached segments.
  */
QRectF CGSegment::boundingRect() const
{
	QRectF bounds = shape().boundingRect();
	return bounds;
}

QPainterPath CGSegment::shape() const
{
	CGSegment* me=(CGSegment*)this;
	QPainterPath ppath;
	for(int n=0; n < me->segments(); n++)
	{
		ppath.addPath(me->segment(n)->shape());
	}
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
