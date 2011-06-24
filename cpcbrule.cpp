/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbrule.h"

#define inherited CSpecctraObject

CPcbRule::CPcbRule(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbRule::~CPcbRule()
{
}


/**
  * @return the default track width
  */
double CPcbRule::width()
{
	double w=15;
	for(int n=0;n<children().count();n++)
	{
		CSpecctraObject* obj = children().at(n);
		if (obj->objectClass()=="width" && obj->properties().count())
		{
			w = obj->properties().at(0).toDouble();
		}
	}
	return w;
}
