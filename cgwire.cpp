/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgwire.h"

#define inherited CGSegment

CGWire::CGWire(QObject *parent)
: inherited(parent)
{
}

CGWire::~CGWire()
{
}

/**
  * @return the shape of the wire segment
  */
QPainterPath CGWire::shape() const
{
	QPainterPath ppath;
	ppath.addPath(inherited::shape());
	return ppath;
}

