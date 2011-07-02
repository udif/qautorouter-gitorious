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
QStringList CPcbNet::pinRefs()
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
		if ( padstack(n)->unitRef() == CUtil::unitRef(ref) && padstack(n)->pinRef() == CUtil::pinRef(ref) )
		{
			return padstack(n);
		}
	}
	return NULL;
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

void CPcbNet::paint(QPainter *painter, const QStyleOptionGraphicsItem* /* option */, QWidget* /* widget */)
{
	QPainterPath p = shape();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->scale(scale(),scale());
	painter->setPen(QPen(QColor(255, 255, 255), 3, Qt::SolidLine,Qt::FlatCap,Qt::MiterJoin));
	painter->drawPath(shape());
}
