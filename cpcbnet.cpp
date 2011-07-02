/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbnet.h"
#include "cpcbpins.h"
#include "cgpadstack.h"
#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcbrule.h"
#include "cpcbnetwork.h"
#include "cpcbclass.h"
#include "cpcbpins.h"
#include "cutil.h"
#include "cpcbplace.h"
#include "cpcbplacement.h"

#define inherited CSpecctraObject

CPcbNet::CPcbNet(QGraphicsItem *parent)
: inherited(parent)
, mRouted(false)
, mWidth(-1.0)
{
	CSpecctraObject::scene()->addItem(this);
}

CPcbNet::~CPcbNet()
{
}

/**
  * @brief clear the chahe
  */
void CPcbNet::clearCache()
{
	QPainterPath empty;
	mShape = empty;
	inherited::clearCache();
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

/**
  * @return the width of the net tracks
  */
double CPcbNet::width()
{
	double w=12;
	if ( mWidth < 0.0 )
	{
		if ( netClass() != NULL )
		{
			w = netClass()->width();
		}
		else if (pcb()!=NULL && pcb()->structure()!=NULL && pcb()->structure()->rule()!=NULL)
		{
			w = pcb()->structure()->rule()->width();
		}
	}
	else
	{
		w = mWidth;
	}
	return w;
}

/**
  * @return enumerated list of pin references in this net.
  */
QStringList& CPcbNet::pinRefs()
{
	if ( mPinRefs.count()==0 )
	{
		CPcbPins* objPins = (CPcbPins*)child("pins");
		if ( objPins != NULL )
		{
			for(int n=0; n < objPins->pinRefs(); n++)
			{
				mPinRefs.append(objPins->pinRef(n));
			}
		}
	}
	return mPinRefs;
}

/**
  * @return the number of pads in the net
  */
int CPcbNet::padstacks()
{
	if ( mPadstacks.count() == 0 )
	{
		for(int n=0; n < pinRefs().count(); n++)
		{
			QString unitRef = CUtil::unitRef(pinRefs().at(n));
			QString pinRef = CUtil::pinRef(pinRefs().at(n));
			CPcbPlace* place = pcb()->placement()->place(unitRef);
			if ( place != NULL )
			{
				CGPadstack* padstack = place->pad(pinRef);
				if ( padstack != NULL )
				{
					mPadstacks.append(padstack);
				}
			}
		}
	}
	return mPadstacks.count();
}

/**
  * @return a padstack by index
  */
CGPadstack* CPcbNet::padstack(int idx)
{
	if ( padstacks() > idx )
	{
		return mPadstacks.at(idx);
	}
	return NULL;
}

/**
  * @return a padstack by UNIT-PIN string format reference
  */
CGPadstack* CPcbNet::padstack(QString ref)
{
	for(int n=0; n < padstacks(); n++)
	{
		if ( padstack(n)->placeRef() == ref )
		{
			return padstack(n);
		}
	}
	return NULL;
}

/**
  * @return a reference to the padstacks list.
  */
QList<CGPadstack*>& CPcbNet::padstacksRef()
{
	padstacks(); /** prime the padstacks list */
	return mPadstacks;
}

/**
  * @brief sort the netlist with respect to distance from a point.
  */
void CPcbNet::sort(QPointF pt,CUtil::tSortOrder order)
{
	clearCache();
	/* sort the padstack lists associated with each net relative to distance from center point of PCB */
	CUtil::sort(padstacksRef(),pt);
	/* redraw */
	CSpecctraObject::scene()->update();
}

/**
  * @return the net class
  */
CPcbClass* CPcbNet::netClass()
{
	if ( pcb()!=NULL && pcb()->network()!= NULL )
	{
		for( int n=0; n < pcb()->network()->netClasses(); n++)
		{
			CPcbClass* pClass = pcb()->network()->netClass(n);
			if ( pClass != NULL )
			{
				QStringList& nets = pClass->nets();
				if (nets.contains(name()))
				{
					return pcb()->network()->netClass(n);
				}
			}
		}
	}
	return NULL;
}

/**
  * @return verbose description
  */
QString CPcbNet::description()
{
	QString rc;
	rc += name()+" ";
	rc += netClass()->description();
	return rc;
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
		for(int n=0; n < me->padstacks(); n++ )
		{
			CGPadstack* pad = me->padstack(n);
			if ( pad != NULL )
			{
				if ( n== 0 )
					me->mShape.moveTo(pad->pos());
				else
					me->mShape.lineTo(pad->pos());
			}
		}
	}
	return me->mShape;
}

void CPcbNet::paint(QPainter *painter, const QStyleOptionGraphicsItem* /* option */, QWidget* /* widget */)
{
	QPainterPath p = shape();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->scale(scale(),scale());
	painter->setPen(QPen(QColor(255, 255, 255), 3, Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
	painter->drawPath(shape());
}
