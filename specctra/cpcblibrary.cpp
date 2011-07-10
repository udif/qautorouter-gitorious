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
	return childCount("image");
}

/**
  * @return a image by index
  */
CPcbImage* CPcbLibrary::image(int idx)
{
	return (CPcbImage*)child("image",idx);
}

/**
  * @return a image by name
  */
CPcbImage* CPcbLibrary::image(QString name)
{
	int cc = childCount("image");
	for(int n=0; n<cc; n++)
	{
		CPcbImage* image = (CPcbImage*)child("image",n);
		if ( image->name() == name )
			return image;
	}
	return NULL;
}

/**
  * @return the padstack count
  */
int CPcbLibrary::padstacks()
{
	return childCount("padstack" );
}

/**
  * @return a padstack by index
  */
CPcbPadstack* CPcbLibrary::padstack(int idx)
{
	return (CPcbPadstack*)child("padstack",idx);
	return NULL;
}

/**
  * @return a padstack by name
  */
CPcbPadstack* CPcbLibrary::padstack(QString name)
{
	int cc = childCount("padstack");
	for(int n=0; n<cc; n++)
	{
		CPcbPadstack* padstack = (CPcbPadstack*)child("padstack",n);
		if ( padstack->name() == name )
			return padstack;
	}
	return NULL;
}
