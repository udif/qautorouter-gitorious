/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbimage.h"

#define inherited CSpecctraObject

CPcbImage::CPcbImage(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbImage::~CPcbImage()
{
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
CPcbOutline* CPcbImage::outline(int comp)
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

	if ( comp < mOutlines.count() )
		return mOutlines.at(comp);

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
CPcbPin* CPcbImage::pin(int comp)
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

	if ( comp < mPins.count() )
		return mPins.at(comp);
	return NULL;
}
