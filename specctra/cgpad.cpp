/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgpad.h"
#include "cgpadstack.h"
#include "cspecctraobject.h"
#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcblayer.h"

#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QColor>

CGPad::CGPad(CGPadstack* padstack, QPainterPath shape, QString layer, QObject *parent)
: QObject(parent)
, mPadstack(padstack)
, mShape(shape)
, mLayer(layer)
, mLayerObject(NULL)
{
	padstack->addPad(this);
	CSpecctraObject::scene()->addItem(this);
	setOpacity(0.45);
}

/**
  * @brief convinience function to get the root pcb pointer.
  */
CPcb* CGPad::pcb()
{
	return padstack()->pcb();
}

QColor CGPad::color()
{
	QColor c(0xa0,0,0);
	if ( mLayerObject==NULL )
	{
		if (pcb()!=NULL && pcb()->structure()!=NULL)
		{
			mLayerObject = pcb()->structure()->layer(layer());
		}
	}
	if ( mLayerObject!=NULL )
	{
		c = mLayerObject->color();
	}
	return c;
}


QString CGPad::pinRef()
{
	return padstack()->pinRef();
}

/**
  * @return center point in absolute coordinates.
  */
QPointF CGPad::pos()
{
	QPointF pt;
	QRectF bounds = mapRectToScene(boundingRect());
	pt.setX(bounds.left()+(bounds.width()/2));
	pt.setY(bounds.top()+(bounds.height()/2));
	return pt;
}

QRectF CGPad::boundingRect() const
{
	CGPad* me = (CGPad*)this;
	if ( me->mBoundingRect.isEmpty() )
	{
		QPainterPath ppath = me->shape();
		me->mBoundingRect = ppath.boundingRect();
	}
	return me->mBoundingRect;
}

QPainterPath CGPad::shape() const
{
	return mShape;
}

void CGPad::paint(QPainter *painter, const QStyleOptionGraphicsItem* /* option */,QWidget* /* widget */)
{
	QPainterPath ppath = shape();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->scale(scale(),scale());
	ppath.setFillRule(Qt::WindingFill);
	painter->setPen(QPen(color(), 1, Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
	painter->drawPath(ppath);
	painter->fillPath(ppath,color());
}

