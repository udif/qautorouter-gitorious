/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbnet.h"
#include "cpcbpins.h"
#include "cgpadstack.h"

#define inherited CSpecctraObject

CPcbNet::CPcbNet(QGraphicsItem *parent)
: inherited(parent)
, mRouted(false)
{
	CSpecctraObject::scene()->addItem(this);
}

CPcbNet::~CPcbNet()
{
}

/**
  * @return the name of the net
  */
QString CPcbNet::name()
{
	QString nm;
	if ( properties().count() )
	{
		nm = properties().at(0);
	}
	return nm;
}


QRectF CPcbNet::boundingRect() const
{
	QRectF bounds = shape().boundingRect();
	return bounds;
}

QPainterPath CPcbNet::shape() const
{
	CPcbNet* me=(CPcbNet*)this;
	if ( me->mShape.isEmpty())
	{
		for( int iPins=0; iPins < me->children().count(); iPins++)
		{
			CSpecctraObject* pObj = (CPcbPins*)me->children().at(iPins);
			if ( pObj->objectClass()=="pins")
			{
				CPcbPins* pins = (CPcbPins*)pObj;
				for(int n=0; n < pins->pinRefs(); n++)
				{
					CGPadstack* padstack = CGPadstack::padstack(pins->pinRef(n));
					if ( padstack != NULL )
					{
						if ( n== 0 )
							me->mShape.moveTo(padstack->pos());
						else
							me->mShape.lineTo(padstack->pos());
					}
				}
			}
		}
	}
	return me->mShape;
}

void CPcbNet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPainterPath p = shape();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->scale(scale(),scale());
	painter->setPen(QPen(QColor(255, 255, 255), 3, Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
	painter->drawPath(shape());
}
