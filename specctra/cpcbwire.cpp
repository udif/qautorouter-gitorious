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

#define inherited CSpecctraObject

CPcbWire::CPcbWire(QGraphicsItem *parent)
: inherited(parent)
{
	CSpecctraObject::globalScene()->addItem(this);
}

CPcbWire::~CPcbWire()
{
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

CPcbPolylinePath* CPcbWire::polylinePath()
{
	return (CPcbPolylinePath*)child("polyline_path");
}

CPcbClearanceClass* CPcbWire::clearanceClass()
{
	return (CPcbClearanceClass*)child("clearance_class");
}

CPcbNet* CPcbWire::net()
{
	return (CPcbNet*)child("net");
}

QRectF CPcbWire::boundingRect() const
{
	CPcbWire* me=(CPcbWire*)this;
	QRectF rect = me->shape().boundingRect();
	return rect;
}

QPainterPath CPcbWire::shape() const
{
	CPcbWire* me=(CPcbWire*)this;
	QPainterPath ppath;
	QPointF pt;
	for(int n=0; n < me->polylinePath()->coords(); n+=2)
	{
		int x = me->polylinePath()->coord(n);
		int y = me->polylinePath()->coord(n+1);
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

void CPcbWire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if ( width() > 0 )
	{
		painter->setRenderHint(QPainter::Antialiasing);
		painter->scale(scale(),scale());
		painter->setPen(QPen(layer()->color(), width(), Qt::SolidLine,Qt::RoundCap,Qt::MiterJoin));
		painter->drawPath(shape());
	}
}

