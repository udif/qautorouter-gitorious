/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcboutline.h"
#include "cpcbimage.h"
#include "cpcbpath.h"

#define inherited CSpecctraObject

CPcbOutline::CPcbOutline(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbOutline::~CPcbOutline()
{
}
/**
  * @return the component count
  */
int CPcbOutline::paths()
{
	return childCount("path");
}

/**
  * @return a component by index
  */
CPcbPath* CPcbOutline::path(int idx)
{
	return (CPcbPath*)child("path",idx);
}

/**
  * @brief path
  */
CPcbPath* CPcbOutline::path()
{
	return path(0);
}

