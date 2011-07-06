/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbcomponent.h"

#define inherited CSpecctraObject

CPcbComponent::CPcbComponent(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbComponent::~CPcbComponent()
{
}

/**
  * @return footprint
  */
QString CPcbComponent::footprint()
{
	QString fp;
	if ( properties().count() )
	{
		fp = properties().at(0);
	}
	return fp;
}

/**
  * @return the component count
  */
int CPcbComponent::places()
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "place" )
		{
			++count;
		}
	}
	return count;
}

/**
  * @return a component by index
  */
CPcbPlace* CPcbComponent::place(int comp)
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "place" )
		{
			if ( count == comp )
			{
				return (CPcbPlace*)children().at(n);
			}
			++count;
		}
	}
	return NULL;
}
