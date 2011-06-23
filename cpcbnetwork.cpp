/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbnetwork.h"
#include "cpcbnet.h"

#define inherited CSpecctraObject

CPcbNetwork::CPcbNetwork(QGraphicsItem *parent)
: inherited(parent)
, nNets(-1)
{
}

CPcbNetwork::~CPcbNetwork()
{
}

/**
  * @return the net count
  */
int CPcbNetwork::nets()
{
	if ( nNets < 0 )
	{
		int count=0;
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "net" )
			{
				++count;
			}
		}
		nNets = count;
	}
	return nNets;
}

/**
  * @return a net by index
  */
CPcbNet* CPcbNetwork::net(int idx)
{
	if ( mNets.count() == 0 )
	{
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "net" )
			{
				mNets.append((CPcbNet*)children().at(n));
			}
		}
	}

	if ( idx < mNets.count() )
		return mNets.at(idx);
	return NULL;
}

/**
  * @return a net by name
  */
CPcbNet* CPcbNetwork::net(QString name)
{
	for(int n=0;n<children().count();n++)
	{
		CSpecctraObject* obj = children().at(0);
		if (obj->objectName()=="net")
		{
			CPcbNet* pcbNet = (CPcbNet*)obj;
			if ( pcbNet->name() == name )
			{
				return pcbNet;
			}
		}
	}
	return NULL;
}


