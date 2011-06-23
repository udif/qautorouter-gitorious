/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcblibrary.h"
#include "cpcbimage.h"

#define inherited CSpecctraObject

CPcbLibrary::CPcbLibrary(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbLibrary::~CPcbLibrary()
{
}

/**
  * @return the image count
  */
int CPcbLibrary::images()
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "image" )
		{
			++count;
		}
	}
	return count;
}

/**
  * @return a image by index
  */
CPcbImage* CPcbLibrary::image(int comp)
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "image" )
		{
			if ( count == comp )
			{
				return (CPcbImage*)children().at(n);
			}
			++count;
		}
	}
	return NULL;
}

/**
  * @return a image by name
  */
CPcbImage* CPcbLibrary::image(QString name)
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		CSpecctraObject* obj = children().at(n);
		if ( obj->objectClass() == "image" )
		{
			CPcbImage* image = (CPcbImage*)obj;
			if ( image->name() == name )
			{
				return image;
			}
			++count;
		}
	}
	return NULL;
}
