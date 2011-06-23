/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbimage.h"

#define inherited CSpecctraObject

CPcbImage::CPcbImage(QGraphicsItem *parent)
: inherited(parent)
, nOutlines(-1)
, nPins(-1)
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
	if ( nOutlines < 0 )
	{
		int count=0;
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "outline" )
			{
				++count;
			}
		}
		nOutlines = count;
	}
	return nOutlines;
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
	if ( nPins < 0 )
	{
		int count=0;
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "pin" )
			{
				++count;
			}
		}
		nPins = count;
	}
	return nPins;
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
