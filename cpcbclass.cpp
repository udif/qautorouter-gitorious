/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbclass.h"

#define inherited CSpecctraObject

CPcbClass::CPcbClass(QGraphicsItem *parent)
: inherited(parent)
, nNets(-1)
{
}

CPcbClass::~CPcbClass()
{
}

/**
  * @return the name of the net class.
  */
QString CPcbClass::name()
{
	QString nm;
	if ( properties().count() )
		nm = properties().at(0);
	return nm;
}

/**
  * @return the net count
  */
int CPcbClass::nets()
{
	if ( nNets < 0 )
	{
		CSpecctraObject* obj = parentObject();
		if (obj->objectName()=="network")
		{
			for( int n=1; n < properties().count(); n++ )
			{
			}
		}

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
CPcbNet* CPcbClass::net(int idx)
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
