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
	return childCount("place");
}

/**
  * @return a component by index
  */
CPcbPlace* CPcbComponent::place(int idx)
{
	return (CPcbPlace*)child("place",idx);
}
