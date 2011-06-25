/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgwire.h"

#define inherited QObject

CGWire::CGWire(QObject *parent)
: inherited(parent)
{
}

CGWire::~CGWire()
{
}

int CGWire::segments()
{
	return mSegments.count();
}

CGSegment* CGWire::segment(int idx)
{
	return mSegments.at(idx);
}


