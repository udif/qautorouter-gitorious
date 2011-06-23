/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbpadstack.h"

#define inherited CSpecctraObject

CPcbPadstack::CPcbPadstack(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbPadstack::~CPcbPadstack()
{
}

QString CPcbPadstack::name()
{
	return properties().at(0);
}

int CPcbPadstack::shapes()
{
	int count = 0;
	for( int n=0; n < children().count(); n++ )
	{
		if ( children().at(n)->objectClass() == "shape")
		{
			++count;
		}
	}
	return count;
}

CPcbShape* CPcbPadstack::shape(int idx)
{
	if ( idx < children().count() )
		return (CPcbShape*)children().at(idx);
	return NULL;
}

