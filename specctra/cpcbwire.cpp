/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbwire.h"
#include "cpcbpath.h"
#include "cpcbnet.h"
#include "cpcbclearanceclass.h"
#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcblayer.h"

#define inherited CPcbSegment

CPcbWire::CPcbWire(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbWire::~CPcbWire()
{
}

/**
  * @return the class of drawable object which this is.
  */
CSpecctraObject::tDrawableClass CPcbWire::drawableClass()
{
	return CSpecctraObject::Track;
}


/**
  * @return boolean indicating the drawability of the object.
  */
bool CPcbWire::drawable()
{
	if ( width() >= 1 )
		return true;
	return false;
}

/**
  * @return a layer index used for Z-axis layering.
  */
double CPcbWire::layerIndex()
{
	if ( pcb() != NULL && pcb()->structure() != NULL )
	{
		CPcbStructure* structure =  pcb()->structure();
		int layers =structure->layers();
		return layers-layer()->index();
	}
	return 0;
}

/**
  * @return the width of the wire.
  */
double CPcbWire::width()
{
	if ( path() != NULL )
		return path()->width();
	return 0.0;
}

QString CPcbWire::layerRef()
{
	QString rc;
	if ( path() != NULL )
		rc = path()->layer();
	return rc;
}

CPcbLayer* CPcbWire::layer()
{
	if ( pcb() != NULL && pcb()->structure() != NULL )
		return pcb()->structure()->layer(layerRef());
	return NULL;
}

QColor CPcbWire::color()
{
	QColor c(0,255,0);
	if ( layer() != NULL )
		c = layer()->color();
	return c;
}

CPcbPath* CPcbWire::path()
{
	return (CPcbPath*)child("path");
}

QPainterPath CPcbWire::shape() const
{
	CPcbWire* me=(CPcbWire*)this;
	QPainterPath ppath;
	if ( me->path() != NULL )
	{
		ppath = me->path()->shape();
	}
	return ppath;
}


