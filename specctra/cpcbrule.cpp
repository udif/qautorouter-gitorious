/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbrule.h"
#include "cpcbwidth.h"

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
CPcbWidth* CPcbRule::width()
{
	return (CPcbWidth*)child("width");
}
