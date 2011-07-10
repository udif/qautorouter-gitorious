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
	return childCount("layer");
}

/**
  * @return a layer by index
  */
CPcbLayer* CPcbStructure::layer(int idx)
{
	return (CPcbLayer*)child("layer",idx);
}

/**
  * @return a layer by reference
  */
CPcbLayer* CPcbStructure::layer(QString& ref)
{
	int cc=childCount("layer");
	for(int n=0; n < cc; n++)
	{
		CPcbLayer* layer = (CPcbLayer*)child("layer",n);
		if ( layer->name() == ref )
		{
			return layer;
		}
	}
	return NULL;
}


/**
  * @return the boundary
  */
CPcbBoundary* CPcbStructure::boundary()
{
	return (CPcbBoundary*)child("boundary");
}

