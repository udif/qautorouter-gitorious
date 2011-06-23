/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcblayer.h"

#define inherited CSpecctraObject

CPcbLayer::CPcbLayer(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbLayer::~CPcbLayer()
{
}

/**
  * @return the later type, signal, etc...
  */
QString CPcbLayer::type()
{
	CSpecctraObject* t = child("type");
	if ( t!=NULL )
	{
		if ( t->properties().count() )
		{
			return t->properties().at(0);
		}
	}
	return "";
}

/**
  * @brief @return the layer index...
  */
int CPcbLayer::index()
{
	CSpecctraObject* i = child("property");
	if ( i!=NULL )
	{
		CSpecctraObject* t = i->child("index");
		if ( t->children().count() )
		{
			return t->properties().at(0).toInt();
		}
	}
	return 0;
}

