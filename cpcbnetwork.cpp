/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbnetwork.h"
#include "cpcbnet.h"
#include "cpcbclass.h"

#define inherited CSpecctraObject

CPcbNetwork::CPcbNetwork(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbNetwork::~CPcbNetwork()
{
}

void CPcbNetwork::clearCache()
{
	mNets.clear();
	mClasses.clear();
	inherited::clearCache();
}

/**
  * @return the net count
  */
int CPcbNetwork::nets()
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
	return mNets.count();
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
CPcbNet* CPcbNetwork::net(QString ref)
{
	for(int n=0; n < nets(); n++)
	{
		CPcbNet* pcbNet = net(n);
		if ( pcbNet->name() == ref )
			return pcbNet;
	}
	return NULL;
}

/**
  * @return a references to the net list
  */
QList<CPcbNet*>& CPcbNetwork::netsRef()
{
	nets(); /* prime the net list */
	return mNets;
}

/**
  * @return the number of net classes
  */
int CPcbNetwork::netClasses()
{
	if ( mClasses.count() == 0 )
	{
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "class" )
			{
				mClasses.append((CPcbClass*)children().at(n));
			}
		}
	}
	return mClasses.count();
}

/**
  * @return net class by index.
  */
CPcbClass* CPcbNetwork::netClass(int idx)
{
	if ( mClasses.count() == 0 )
	{
		for(int n=0;n<children().count();n++)
		{
			if ( children().at(n)->objectClass() == "class" )
			{
				mClasses.append((CPcbClass*)children().at(n));
			}
		}
	}

	if ( idx < mClasses.count() )
		return mClasses.at(idx);
	return NULL;
}

/**
  * @erturn net class by reference.
  */
CPcbClass* CPcbNetwork::netClass(QString ref)
{
	for(int n=0; n < netClasses(); n++)
	{
		if ( netClass(n)->name() == ref)
			return netClass(n);
	}
	return NULL;
}


/**
  * @return tally up the unrouted
  */
int CPcbNetwork::routed()
{
	int tot=0;
	for(int n=0; n < nets(); n++)
	{
		if ( net(n)->routed() )
		{
			++tot;
		}
	}
	return tot;
}

