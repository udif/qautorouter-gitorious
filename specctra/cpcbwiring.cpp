/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbwiring.h"
#include "cpcbwire.h"

#define inherited CSpecctraObject

CPcbWiring::CPcbWiring(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbWiring::~CPcbWiring()
{
}

/**
  * @return the wires count
  */
int CPcbWiring::wires()
{
	return childCount("wire");
}

/**
  * @return a wire by index
  */
CPcbWire* CPcbWiring::wire(int idx)
{
	return (CPcbWire*)child("wire",idx);
}

