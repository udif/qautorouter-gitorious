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
#include "cgwire.h"
#include "cgsegment.h"
#include "cpcbwidth.h"

#include <stdio.h>

#define inherited CSpecctraObject

CPcbNet::CPcbNet(QGraphicsItem *parent)
: inherited(parent)
, mWidth(-1.0)
{
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	mWire = new CGWire(this);
	CSpecctraObject::globalScene()->addItem(this);
}

CPcbNet::~CPcbNet()
{
	//delete mWire;
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
  * @brief determine of the net is routed.
  */
bool CPcbNet::routed()
{
	bool rc=true;
	CGWire& w = wire();
	for(int n=0; n < w.segments(); n++)
	{
		CGSegment* seg = w.segment(n);
		if ( !seg->routed() )
		{
			rc=false;
			break;
		}
	}
	return rc;
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
			w = pcb()->structure()->rule()->width()->data();
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
		if ( pcb() != NULL && pcb()->placement() != NULL )
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
		if ( padstack(n)->unitRef() == ref )
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
  * @return the closest padstack
  */
CGPadstack* CPcbNet::closest(int n,QPointF pt)
{
	CGPadstack* close=NULL;
	for(; n < padstacks(); n++)
	{
		CGPadstack* pad = padstack(n);
		if ( close == NULL )
		{
			close = pad;
		}
		else
		{
			QPointF p1 = pad->origin() - pt;
			QPointF p2 = close->origin() - pt;
			double len1 = p1.manhattanLength();
			double len2 = p2.manhattanLength();
			if ( len1 < len2 )
			{
				close = pad;
			}
		}
	}
	return close;
}
/**
  * @return the farthest padstack
  */
CGPadstack* CPcbNet::farthest(int n, QPointF pt)
{
	CGPadstack* far=NULL;
	for(; n < padstacks(); n++)
	{
		CGPadstack* pad = padstack(n);
		if ( far == NULL )
		{
			far = pad;
		}
		else
		{
			QPointF p1 = pad->origin() - pt;
			QPointF p2 = far->origin() - pt;
			double len1 = p1.manhattanLength();
			double len2 = p2.manhattanLength();
			if ( len1 > len2 )
			{
				far = pad;
			}
		}
	}
	return far;
}

/**
  * @brief exchange one padstack with another.
  */
void CPcbNet::swap(CGPadstack* p1,CGPadstack* p2)
{
	if ( p1 != p2)
	{
		int idx1 = padstacksRef().indexOf(p1);
		int idx2 = padstacksRef().indexOf(p2);
		if ( idx1 >= 0 && idx2 >= 0 )
		{
			padstacksRef().swap(idx1,idx2);
		}
	}
}

/**
  * @brief sort the netlist with respect to distance from a point.
  */
void CPcbNet::sort()
{
	/* sort the padstack lists associated with each net relative to distance from center point of PCB */
	for(int n=1; n < padstacks(); n++ )
	{
		CGPadstack* pad = closest(n,padstack(n-1)->origin());
		swap(pad,padstack(n));
	}
}

void CPcbNet::dumpLength()
{
	printf( "********\n");
	for(int n=0; n < padstacks(); n++)
	{
		QPointF pt = padstack(n)->origin() - padstack(0)->origin();
		double len = pt.manhattanLength();
		printf( "%s %g+%g\t%g\n", padstack(n)->unitRef().toAscii().data(), pt.x(),pt.y(), len);
	}
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
  * @brief create a wire if one does not exisst already.
  * @return a reference to the wire.
  */
CGWire& CPcbNet::wire()
{
	if ( mWire->isEmpty() )
	{
		CGSegment* obj = NULL;
		for( int n=0; n < this->padstacks(); n++)
		{
			CGPadstack* pad = padstack(n);
			if ( n == 0 )
			{
				obj = mWire;
			}
			else
			{
				CGWire* child = new CGWire(this);
				obj->append(child);
				obj = child;
			}
			obj->setOrigin(pad->origin());
			obj->append(pad);
			obj = pad;
		}
	}
	return *mWire;
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
#if 1
	QPainterPath p;
	CPcbNet* me=(CPcbNet*)this;
	me->wire(); /* prime the wire segments */
	return p;
#else
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
#endif
}

void CPcbNet::paint(QPainter *painter, const QStyleOptionGraphicsItem* /* option */, QWidget* /* widget */)
{
	QPainterPath p = shape();
#if 0
	painter->setRenderHint(QPainter::Antialiasing);
	painter->scale(scale(),scale());
	painter->setPen(QPen(QColor(255, 255, 255), 3, Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
	painter->drawPath(shape());
#endif
}
