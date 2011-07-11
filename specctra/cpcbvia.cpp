/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbvia.h"
#include "cpcb.h"
#include "cpcblibrary.h"
#include "cpcbpadstack.h"
#include "cpcbshape.h"

#include <stdio.h>

#define inherited CPcbSegment

CPcbVia::CPcbVia(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbVia::~CPcbVia()
{
}

/**
  * @return a layer index used for Z-axis layering.
  */
double CPcbVia::layerIndex()
{
	return 100;
}

bool CPcbVia::drawable()
{
	if ( properties().count() >= 3 && parentObject() != NULL ) // do we have coordinates?
		if ( parentObject()->objectClass() == "wiring" )
			return true;
	return false;
}

QPointF CPcbVia::center()
{
	QPointF pt;
	if ( drawable() )
		pt = QPointF(properties()[1].toDouble(),properties()[2].toDouble());
	return pt;
}

double CPcbVia::width()
{
	return 1.0;
}

QColor CPcbVia::color()
{
	/** FIXME this should combine the colors of the layers of the padstack */
	QColor c(207, 181, 59);
	return c;
}

/**
  * @return a pointer to the padstack object
  */
CPcbPadstack* CPcbVia::padstack()
{
	CPcbPadstack* ps=NULL;
	if ( pcb() != NULL && pcb()->library() != NULL && properties().count() )
	{
		ps = pcb()->library()->padstack(properties().at(0));
	}
	return ps;
}

QPainterPath CPcbVia::shape() const
{
	CPcbVia* me=(CPcbVia*)this;
	QPainterPath ppath;
	if ( me->drawable() )
	{
		if ( me->padstack() != NULL )
		{
			for(int n=0; n < me->padstack()->shapes(); n++)
			{
				QPainterPath other = me->padstack()->shape(n)->shape();
				ppath.addPath(other);
			}
		}
		ppath.translate(me->center());
	}
	return ppath;
}

void CPcbVia::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	if ( drawable() )
	{
		QPainterPath ppath = shape();
		painter->setRenderHint(QPainter::Antialiasing);
		painter->scale(scale(),scale());
		ppath.setFillRule(Qt::WindingFill);
		painter->setPen(QPen(color(), width(), Qt::SolidLine,Qt::RoundCap));
		painter->drawPath(ppath);
		painter->fillPath(ppath,color());
	}
}

