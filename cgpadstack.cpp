/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgpadstack.h"
#include "cpcbplace.h"
#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcblayer.h"

QMap<QString,CGPadstack*>	CGPadstack::mGPadstacks; /** indexed by "UNIT-PIN" string notation. */

CGPadstack::CGPadstack(CPcbPlace* place,QString pinRef, QObject *parent)
: QObject(parent)
, mPlace(place)
, mPinRef(pinRef)
{
	mGPadstacks.insert(unitRef(),this);
}

CGPadstack::~CGPadstack()
{
	QList<QString> keys = mPads.keys();
	for(int n=0; n < keys.count(); n++)
	{
		delete mPads[keys[n]];
	}
}

/**
  * @brief (static) clear the pastack statics
  */
void CGPadstack::clear()
{
	QList<QString> keys = mGPadstacks.keys();
	for(int n=0; n < keys.count(); n++)
	{
		CGPadstack* padstack = mGPadstacks.take(keys[n]);
		delete padstack;
	}
}

/**
  * @brief convinience function to get the root pcb pointer
  */
CPcb* CGPadstack::pcb()
{
	return place()->pcb();
}

/**
  * @brief Add a pad to the padstack.
  */
void CGPadstack::addPad(CGPad* pad)
{
	mPads.insert(pad->layer(),pad);
	/** FIXME HACK */
	if ( pad->layer() == "Copper" )
	{
		for(int n=0; n < pcb()->structure()->layers().count(); n++)
		{
			if (pcb()->structure()->layers().at(n)->name() == "Copper" )
			{
				pcb()->structure()->layers().at(n)->setColor(QColor(0,0xA0,0));
			}
		}
	}
}

/**
  * @return a pointer to a CGpadstack object by unit reference notation "UNIT-PIN".
  */
CGPadstack* CGPadstack::padstack(QString unitRef)
{
	if ( padstacks().contains(unitRef) )
	{
		return padstacks()[unitRef];
	}
	return NULL;
}

/**
  * @return UNIT-PIN notation.
  */
QString CGPadstack::unitRef()
{
	QString rc = place()->unit()+"-"+pinRef();
	return rc;
}

CPcbPlace* CGPadstack::place()
{
	return mPlace;
}

/**
  * @param layer name of layer
  * @return a layer by name
  */
CGPad* CGPadstack::pad(QString layer)
{
	if ( mPads.contains(layer) )
	{
		return mPads[layer];
	}
	return NULL;
}

/**
  * @return a point which is the center of the padstack
  */
QPointF CGPadstack::pos()
{
	QPointF pt;
	for(int n=0; n < layers().count(); n++)
	{
		CGPad* pPad = pad(layers().at(n));
		pt = pPad->pos();
	}
	return pt;
}

/**
  * @return the names of the layers that have pads
  */
QStringList CGPadstack::layers()
{
	QStringList rc;
	QMap<QString, CGPad*>::iterator i;
	for (i = mPads.begin(); i != mPads.end(); ++i)
		rc.append( i.key());
	return rc;
}

/**
  * @return the pin reference for this padstack.
  */
QString CGPadstack::pinRef()
{
	return mPinRef;
}

