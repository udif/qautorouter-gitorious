/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgpadstack.h"
#include "cpcbplace.h"

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
	if ( mGPadstacks.contains(unitRef()) )
	{
		mGPadstacks.take(unitRef());
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

