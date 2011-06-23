/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbshape.h"
#include "cpcbcircle.h"
#include "cpcbrect.h"
#include "cpcbpath.h"

#define inherited CSpecctraObject

CPcbShape::CPcbShape(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbShape::~CPcbShape()
{
}

QString CPcbShape::layer()
{
	QString sLayer;
	CSpecctraObject* child = children().at(0);
	if ( child->objectClass() == "circle" )
	{
		CPcbCircle* circle = (CPcbCircle*)child;
		sLayer = circle->layer();
	}
	else if ( child->objectClass() == "rect")
	{
		CPcbRect* rect = (CPcbRect*)child;
		sLayer = rect->layer();
	}
	else if ( child->objectClass() == "path")
	{
		CPcbPath* path = (CPcbPath*)child;
		sLayer = path->layer();
	}
	return sLayer;
}

QPainterPath CPcbShape::shape()
{
	QPainterPath ppath;
	CSpecctraObject* child = children().at(0);
	if ( child->objectClass() == "circle")
	{
		CPcbCircle* circle = (CPcbCircle*)child;
		ppath = circle->path();
	}
	else if ( child->objectClass() == "rect")
	{
		CPcbRect* rect = (CPcbRect*)child;
		ppath = rect->path();
	}
	else if ( child->objectClass() == "path")
	{
		CPcbPath* path = (CPcbPath*)child;
		ppath = path->oval();
	}
	return ppath;
}

