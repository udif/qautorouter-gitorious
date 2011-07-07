/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbstructure.h"
#include "cpcbboundary.h"
#include "cpcblayer.h"
#include "cpcbrule.h"

#define inherited CSpecctraObject

CPcbStructure::CPcbStructure(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbStructure::~CPcbStructure()
{
}

/**
  * @return the number of layers
 */
int CPcbStructure::layers()
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "layer" )
		{
			++count;
		}
	}
	return count;
}

/**
  * @return a layer by index
  */
CPcbLayer* CPcbStructure::layer(int idx)
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "layer" )
		{
			if ( count == idx )
			{
				return (CPcbLayer*)children().at(n);
			}
			++count;
		}
	}
	return NULL;
}

/**
  * @return a layer by reference
  */
CPcbLayer* CPcbStructure::layer(QString& ref)
{
	for(int n=0;n<children().count();n++)
	{
		CSpecctraObject* obj = children().at(n);
		if ( obj->objectClass() == "layer" )
		{
			CPcbLayer* layer = (CPcbLayer*)obj;
			if ( layer->name() == ref )
			{
				return layer;
			}
		}
	}
	return NULL;
}


/**
  * @return the boundary
  */
CPcbBoundary* CPcbStructure::boundary()
{
	CSpecctraObject* obj = child("boundary");
	if ( obj != NULL )
	{
		return (CPcbBoundary*)obj;
	}
	return NULL;
}

