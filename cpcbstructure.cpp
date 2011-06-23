/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbstructure.h"
#include "cpcbboundary.h"

#define inherited CSpecctraObject

CPcbStructure::CPcbStructure(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbStructure::~CPcbStructure()
{
}

/**
  * @brief Append child objects, and interpret the interestin gones.
  */
void CPcbStructure::appendChild(CSpecctraObject* child)
{
	inherited::appendChild(child);
	if ( child->objectClass() == "layer" )
	{
		mLayers.append((CPcbLayer*)child);
	}
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

