/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "boundingbox.h"

BoundingBox::BoundingBox(CPcb* pcb, CPcbNet* net, CGSegment* segment, QObject* parent)
: QObject(parent)
, mPcb(pcb)
, mNet(net)
, mSegment(segment)
{
}

BoundingBox::BoundingBox(const BoundingBox& other)
{
	copy(other);
}

BoundingBox::~BoundingBox()
{
}

/**
  * Copy from another.
  */
BoundingBox& BoundingBox::copy(const BoundingBox& other)
{
	BoundingBox* p = (BoundingBox*)&other;
	setParent(p->parent());
	mPcb = p->pcb();
	mNet = p->net();
	mSegment = p->segment();
	return *this;
}

/**
  * @brief Grow the bounding box
  * @return a rect of the new size or equal to current rect if no growth is possible.
  */
QRectF BoundingBox::grow()
{
	QRectF previousRect = rect();
	QRectF r = rect();
	double amount = segment()->width();
	r.setX(r.x()-amount);
	r.setY(r.y()-amount);
	r.setWidth(r.width()+amount);
	r.setHeight(r.height()+amount);
	/* have we gone completely outsize the bounds of the pcb? */
	if ( r.contains( pcb()->boundingRect() ) )
	{
		/** limit to the bounds of the PCB. */
		r = pcb()->boundingRect();
	}
	return r;
}

/**
  * @brief route a segment
  */
void BoundingBox::route(CPcbNet* net, CGSegment* segment)
{
	segment->setWidth(20);
	segment->setRouted(true);
	segment->update();
	pcb()->yield();				/* give up some CPU  */
	for(int n=0; n < segment->segments(); n++)
	{
		CGSegment* child = segment->segment(n);
		route(net,child);
	}
}

/**
  * @brief attempt to route the segment
  */
void BoundingBox::route()
{
	route(net(),segment());
}

