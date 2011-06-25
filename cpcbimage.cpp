/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbimage.h"
#include "cpcboutline.h"
#include "cpcbpin.h"

#define inherited CSpecctraObject

CPcbImage::CPcbImage(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbImage::~CPcbImage()
{
}

void CPcbImage::clearCache()
{
	mOutlines.clear();
	mPins.clear();
	inherited::clearCache();
}


QString CPcbImage::name()
{
	return properties().at(0);
}

/**
  * @return the outline count
  */
int CPcbImage::outlines()
{
	if ( mOutlines.count() == 0 )
	{
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "outline" )
			{
				mOutlines.append((CPcbOutline*)children().at(n));
			}
		}
	}
	return mOutlines.count();
}

/**
  * @return a outline by index
  */
CPcbOutline* CPcbImage::outline(int idx)
{
	if ( mOutlines.count() == 0 )
	{
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "outline" )
			{
				mOutlines.append((CPcbOutline*)children().at(n));
			}
		}
	}

	if ( idx < mOutlines.count() )
		return mOutlines.at(idx);
	return NULL;
}

/**
  * @return the pin count
  */
int CPcbImage::pins()
{
	if ( mPins.count() == 0 )
	{
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "pin" )
			{
				mPins.append((CPcbPin*)children().at(n));
			}
		}
	}
	return mPins.count();
}

/**
  * @return a pin by index
  */
CPcbPin* CPcbImage::pin(int idx)
{
	if ( mPins.count() == 0 )
	{
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "pin" )
			{
				mPins.append((CPcbPin*)children().at(n));
			}
		}
	}
	if ( idx < mPins.count() )
		return mPins.at(idx);
	return NULL;
}

/**
  * @return a pin by reference.
  */
CPcbPin* CPcbImage::pin(QString ref)
{
	for(int n=0; n < pins(); n++)
	{
		if ( pin(n)->name() == ref)
			return pin(n);
	}
	return NULL;
}
