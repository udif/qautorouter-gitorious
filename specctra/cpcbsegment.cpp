/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbsegment.h"
#include "cpcblayer.h"
#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcbboundary.h"

#define inherited CSpecctraObject

CPcbSegment::CPcbSegment(QGraphicsItem *parent)
: inherited(parent)
{
	CSpecctraObject::globalScene()->addItem(this);
}

CPcbSegment::~CPcbSegment()
{
}

bool CPcbSegment::drawable()
{
	return true;
}

CPcbClearanceClass* CPcbSegment::clearanceClass()
{
	return (CPcbClearanceClass*)child("clearance_class");
}

CPcbNet* CPcbSegment::net()
{
	return (CPcbNet*)child("net");
}

QRectF CPcbSegment::boundingRect() const
{
	CPcbSegment* me=(CPcbSegment*)this;
	QRectF rect;
	if ( me->drawable() )
	{
		rect = me->shape().boundingRect();
	}
	else
	{
		if ( me->pcb() != NULL && me->pcb()->structure() != NULL && me->pcb()->structure()->boundary() != NULL )
		{
			QPointF topLeft = me->pcb()->structure()->boundary()->boundingRect().topLeft();
			rect.setTopLeft(topLeft);
			rect.setWidth(0);
			rect.setHeight(0);
		}
	}
	return rect;
}

void CPcbSegment::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if ( drawable() )
	{
		painter->setRenderHint(QPainter::Antialiasing);
		painter->scale(scale(),scale());
		painter->setPen(QPen(color(), width(), Qt::SolidLine,Qt::RoundCap));
		painter->drawPath(shape());
	}
}

