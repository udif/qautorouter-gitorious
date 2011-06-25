/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgsegment.h"

#define inherited QObject

CGSegment::CGSegment(QObject *parent)
: inherited(parent)
{
}

CGSegment::~CGSegment()
{
}

QPainterPath CGSegment::shape()
{
	QPainterPath ppath;
	// FIXME - do stuff here
	return ppath;
}
