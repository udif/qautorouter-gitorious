/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbclass.h"
#include "cpcbnet.h"
#include "cpcbrule.h"

#define inherited CSpecctraObject

CPcbClass::CPcbClass(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbClass::~CPcbClass()
{
}

/**
  * @brief clear the micro-cache of this and children
  */
void CPcbClass::clearCache()
{
	mNets.clear();
	inherited::clearCache();
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
  * @return a verbose description.
  */
QString CPcbClass::description()
{
	QString rc;
	rc += name()+" ";
	rc += QString::number(width())+"mil";
	return rc;
}

/**
  * @return the net count
  */
QStringList& CPcbClass::nets()
{
	if ( mNets.count() == 0 )
	{
		for( int n=1; n < properties().count(); n++)
		{
			mNets.append(properties().at(n));
		}
	}
	return mNets;
}

/**
  * @return the track width
  */
double CPcbClass::width()
{
	double w=12;
	CPcbRule* r = rule();
	if ( r != NULL )
		w = r->width();
	return w;
}
