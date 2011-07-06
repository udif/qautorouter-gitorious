/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbpath.h"

#include <QPointF>

#define inherited CSpecctraObject

CPcbPath::CPcbPath(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbPath::~CPcbPath()
{
}

/**
  * @return the surface layer
  */
QString CPcbPath::layer()
{
	QString lyr;
	if ( properties().count() )
	{
		lyr = properties().at(0);
	}
	return lyr;
}

/**
  * @return the track width.
  */
double CPcbPath::width()
{
	int w;
	if ( properties().count() )
	{
		w = properties().at(1).toDouble();
	}
	return w;
}

/**
  * @return the polygon
  */
QPainterPath CPcbPath::shape()
{
	QPainterPath ppath;
	QPointF startPt(properties().at(2).toDouble(),properties().at(3).toDouble());
	ppath.moveTo(startPt);
	for(int n=4; n < properties().count(); n+=2)
	{
		QPointF pt(properties().at(n).toDouble(),properties().at(n+1).toDouble());
		ppath.lineTo(pt);
	}
	ppath.lineTo(startPt);
	return ppath;
}

/**
  * @return the oval polygon (get's special interpretation)
  */
QPainterPath CPcbPath::oval()
{
	double radius = width()/2.0;
	QPainterPath ppath;
	QPointF centerPt1(properties().at(2).toDouble(),properties().at(3).toDouble());
	QPointF centerPt2(properties().at(4).toDouble(),properties().at(5).toDouble());
	if ( centerPt1.x() != centerPt2.x() )
	{
		ppath.moveTo(QPointF(centerPt1.x(),centerPt1.y()-radius));
		ppath.lineTo(QPointF(centerPt2.x(),centerPt2.y()-radius));
		ppath.moveTo(QPointF(centerPt1.x(),centerPt1.y()+radius));
		ppath.lineTo(QPointF(centerPt2.x(),centerPt2.y()+radius));
	}
	else
	{
		ppath.moveTo(QPointF(centerPt1.x()-radius,centerPt1.y()));
		ppath.lineTo(QPointF(centerPt2.x()-radius,centerPt2.y()));
		ppath.moveTo(QPointF(centerPt1.x()+radius,centerPt1.y()));
		ppath.lineTo(QPointF(centerPt2.x()+radius,centerPt2.y()));
	}
	ppath.connectPath(circle(centerPt1,radius));
	ppath.connectPath(circle(centerPt2,radius));

	return ppath;
}

/**
  * @brief draw a circle as part of oval pad drawing.
  */
QPainterPath CPcbPath::circle(QPointF center,double radius)
{
	QPainterPath ppath;
	QPointF topLeft(center.x()-radius,center.y()-radius);
	QPointF bottomRight(center.x()+radius,center.y()+radius);
	ppath.addEllipse(QRectF(topLeft,bottomRight));
	return ppath;
}
