/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcblibrary.h"
#include "cpcbimage.h"
#include "cpcbpadstack.h"

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
		}
	}
	return NULL;
}

/**
  * @return the padstack count
  */
int CPcbLibrary::padstacks()
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "padstack" )
		{
			++count;
		}
	}
	return count;
}

/**
  * @return a padstack by index
  */
CPcbPadstack* CPcbLibrary::padstack(int idx)
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "padstack" )
		{
			if ( count == idx )
			{
				return (CPcbPadstack*)children().at(n);
			}
			++count;
		}
	}
	return NULL;
}

/**
  * @return a padstack by name
  */
CPcbPadstack* CPcbLibrary::padstack(QString name)
{
	for(int n=0;n<children().count();n++)
	{
		CSpecctraObject* obj = children().at(n);
		if ( obj->objectClass() == "padstack" )
		{
			CPcbPadstack* padstack = (CPcbPadstack*)obj;
			if ( padstack->name() == name )
			{
				return padstack;
			}
		}
	}
	return NULL;
}
