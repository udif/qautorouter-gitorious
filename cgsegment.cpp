/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgsegment.h"
#include "cspecctraobject.h"

#include <QPainter>

#define inherited QObject

CGSegment::CGSegment(QObject *parent)
: inherited(parent)
, mWidth(0.0)
, mLayer(NULL)
, mParentSegment(NULL)
{
	CSpecctraObject::scene()->addItem(this);
}

CGSegment::~CGSegment()
{
	clear();
}

/**
  * @brief clear
  */
void CGSegment::clear()
{
	for(int n=0; n < mSegments.count(); n++)
	{
		if ( mSegments.at(n)->isA(CGSegment::Padstack) && !mSegments.at(n)->isA(CGSegment::Via) ) /* is it a placed pad? */
		{
			mSegments.at(n)->clear();					/* yes...skip it... */
			mSegments.at(n)->setParentSegment(NULL);	/* ...and disconnect it from the wire */
		}
		else
			delete mSegments.at(n);						/* no, delete it */
	}
	mSegments.clear();
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

/**
  * @brief set the pointer to the segment layer.
  */
void CGSegment::setLayer(CPcbLayer* layer)
{
	mLayer=layer;
}

/**
  * @return the a pointer to the segment layer.
  */
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
	segment->setParentSegment(this);
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

/**
  * @return the parent position point or of parent is NULL, then this position pont.
  */
QPointF CGSegment::parentOrigin()
{
	if ( parentSegment() != NULL )
	{
		return parentSegment()->origin();
	}
	return origin();
}

/**
  * @return the segment shape.
  */
QPainterPath CGSegment::shape() const
{
	CGSegment* me=(CGSegment*)this;
	QPainterPath ppath;
	ppath.moveTo(me->parentOrigin());
	ppath.lineTo(me->origin());
	return ppath;
}

void CGSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem* /* option */, QWidget* /* widget */)
{
	QPainterPath p = shape();
	if ( !p.isEmpty() )
	{
		p = shape();
		painter->setRenderHint(QPainter::Antialiasing);
		painter->scale(scale(),scale());
		painter->setPen(QPen(QColor(0, 0, 0), 3, Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
		painter->drawPath(shape());
	}
}
