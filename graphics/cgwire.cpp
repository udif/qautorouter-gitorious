/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cgwire.h"
#include "cgvia.h"

#include <QMessageBox>

#define inherited CGSegment

CGWire::CGWire(CPcbNet* net)
: inherited(net)
{
}

CGWire::~CGWire()
{
}

CPcbNet* CGWire::net()
{
	if ( mNet != NULL )
	{
		return mNet;
	}
	return inherited::net();
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

/**
  * @brief break the wire into another segment at point
  */
bool CGWire::insertBreak(QPointF pt,CGSegment::tSegment style)
{
	bool rc=false;
	CGSegment* seg=NULL;
	switch(style)
	{
	default:
		QMessageBox::warning(NULL,tr("Invalid Segment"),tr("Program Error: CGWire::insertBreak(")+QString::number((int)style)+tr("): An invalid segment style was requested."));
		break;
	case CGSegment::Wire:
		seg = new CGWire(this->net());
		rc=true;
		break;
	case CGSegment::Via:
		seg = new CGVia(this->net());
		rc=true;
		break;
	}
	if ( seg != NULL )
	{
		for(int n=0; n < segmentsRef().count(); n++)
		{
			CGSegment* child = segmentsRef().at(n);
			seg->append(child);
		}
		segmentsRef().clear();
		append(seg);
	}
	return rc;
}
