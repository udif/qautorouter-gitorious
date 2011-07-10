/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbwire.h"
#include "cpcbpolylinepath.h"
#include "cpcbnet.h"
#include "cpcbclearanceclass.h"

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

QString CPcbWire::layer()
{
	QString rc;
	if ( polylinePath() != NULL )
		rc = polylinePath()->layer();
	return rc;
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
	QRectF rect;
	if ( me->polylinePath() != NULL )
		rect = me->polylinePath()->boundingRect();
	return rect;
}

QPainterPath CPcbWire::shape() const
{
	CPcbWire* me=(CPcbWire*)this;
	QPainterPath ppath;
	if ( me->polylinePath() != NULL )
		ppath = me->polylinePath()->shape();
	return ppath;
}

void CPcbWire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if ( width() > 0 )
	{
		painter->setRenderHint(QPainter::Antialiasing);
		painter->scale(scale(),scale());
		painter->setPen(QPen(QColor(0,255,0), width(), Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
		painter->drawPath(shape());
	}
}

