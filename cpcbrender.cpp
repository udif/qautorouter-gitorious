/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbrender.h"
#include "cpcbpath.h"
#include "cpcbplacement.h"
#include "cpcbcomponent.h"
#include "cpcblibrary.h"
#include "cpcbimage.h"
#include "cpcboutline.h"
#include "cpcbplace.h"
#include "cpcbstructure.h"
#include "cpcbboundary.h"
#include "cpcbpin.h"
#include "cpcbpadstack.h"
#include "cpcbshape.h"

#include <QGraphicsPolygonItem>
#include <QPainterPath>

CPcbRender::CPcbRender(CPcb* pcb, QGraphicsView* view, QObject *parent)
: QObject(parent)
, mPcb(pcb)
, mPcbView(view)
{
}

void CPcbRender::renderOutline()
{
	QGraphicsPathItem* pathItem = scene()->addPath(mPcb->structure()->boundary()->path()->shape(),QColor(255,255,0));
}

void CPcbRender::renderOutlines()
{
	int nComponents = mPcb->placement()->components();
	for(int iComponent=0; iComponent < nComponents; iComponent++)
	{
		CPcbComponent* component = mPcb->placement()->component(iComponent);
		int nPlaces = component->places();
		CPcbImage* image = mPcb->library()->image(component->footprint());
		if ( image != NULL )
		{
			for(int iPlace=0; iPlace < nPlaces; iPlace++)
			{
				CPcbPlace* place = component->place(iPlace);
				int nOutlines = image->outlines();
				for(int iOutline=0; iOutline < nOutlines; iOutline++)
				{
					CPcbOutline* outline = image->outline(iOutline);
					int nPaths = outline->paths();
					QPainterPath p;
					for(int iPath=0;iPath < nPaths; iPath++)
					{
						CPcbPath* path = outline->path(iPath);
						p.addPath(path->shape());
					}
					QPointF pt = place->pos();
					p.translate(pt);
					QGraphicsPathItem* pathItem = scene()->addPath(p,QColor(224,255,255));
					pathItem->setTransformOriginPoint(pt);
					pathItem->setRotation(place->rotation());
				}
			}
		}
		else
				emit fault("footprint '"+component->footprint()+"' not found.");
	}
}

void CPcbRender::renderPins()
{
	int nComponents = mPcb->placement()->components();
	for(int iComponent=0; iComponent < nComponents; iComponent++)
	{
		CPcbComponent* component = mPcb->placement()->component(iComponent);
		int nPlaces = component->places();
		CPcbImage* image = mPcb->library()->image(component->footprint());
		if ( image != NULL )
		{
			for(int iPlace=0; iPlace < nPlaces; iPlace++)
			{
				CPcbPlace* place = component->place(iPlace);
				int nPins = image->pins();
				for(int iPin=0; iPin < nPins; iPin++)
				{
					CPcbPin* pin = image->pin(iPin);
					CPcbPadstack* padstack = pin->padstack();
					QPainterPath p;
					int nShapes = padstack->shapes();
					for( int iShape=0; iShape < nShapes; iShape++ )
					{
						CPcbShape* shape = padstack->shape(iShape);
						QPainterPath pShape = shape->shape();
						p.addPath(pShape);
					}
					p.translate(pin->pos());
					p.translate(place->pos());
					QGraphicsPathItem* pathItem = scene()->addPath(p,QColor(255,0,0),QBrush(QColor(255,0,0),Qt::SolidPattern));
					pathItem->setTransformOriginPoint(place->pos());
					pathItem->setRotation(place->rotation());
				}
			}
		}
		else
				emit fault("footprint '"+component->footprint()+"' not found.");
	}
}
















