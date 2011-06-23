/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcb.h"

#define inherited CSpecctraObject

CPcb::CPcb(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcb::~CPcb()
{
}

/**
  * @return the pcb structure object
  */
CPcbStructure* CPcb::structure()
{
	CSpecctraObject* i = child("structure");
	if ( i != NULL )
	{
		return (CPcbStructure*)i;
	}
	return NULL;

}

/**
  * @return the pcb placement object
  */
CPcbPlacement* CPcb::placement()
{
	CSpecctraObject* i = child("placement");
	if ( i != NULL )
	{
		return (CPcbPlacement*)i;
	}
	return NULL;

}
/**
  * @return the pcb library object
  */
CPcbLibrary* CPcb::library()
{
	CSpecctraObject* i = child("library");
	if ( i != NULL )
	{
		return (CPcbLibrary*)i;
	}
	return NULL;

}

