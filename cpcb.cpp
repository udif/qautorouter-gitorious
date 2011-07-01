/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcbplacement.h"
#include "cpcblibrary.h"
#include "cpcbnetwork.h"

#define inherited CSpecctraObject

CPcb::CPcb(QGraphicsItem *parent)
: inherited(parent)
, mStructure(NULL)
, mPlacement(NULL)
, mLibrary(NULL)
, mNetwork(NULL)

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
	if ( mStructure == NULL )
	{
		mStructure = (CPcbStructure*)child("structure");
	}
	return mStructure;
}

/**
  * @return the pcb placement object
  */
CPcbPlacement* CPcb::placement()
{
	if ( mPlacement == NULL )
	{
		mPlacement = (CPcbPlacement*)child("placement");
	}
	return mPlacement;
}

/**
  * @return the pcb library object
  */
CPcbLibrary* CPcb::library()
{
	if ( mLibrary == NULL )
	{
		mLibrary = (CPcbLibrary*)child("library");
	}
	return mLibrary;
}

/**
  * @return the pcb network object
  */
CPcbNetwork* CPcb::network()
{
	if ( mNetwork == NULL )
	{
		mNetwork = (CPcbNetwork*)child("network");
	}
	return mNetwork;
}

