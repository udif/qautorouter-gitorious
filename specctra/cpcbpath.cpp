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
	//path.lineTo(startPt);
	return ppath;
}

/**
  * @return the oval polygon (get's special interpretation)
  */
QPainterPath CPcbPath::oval()
{
	if ( mOval.isEmpty() )
	{
		double radius = width()/2.0;
		QPointF centerPt1(properties().at(2).toDouble(),properties().at(3).toDouble());
		QPointF centerPt2(properties().at(4).toDouble(),properties().at(5).toDouble());
		if ( centerPt1.x() != centerPt2.x() )
		{
			mOval.moveTo(QPointF(centerPt1.x(),centerPt1.y()-radius));
			mOval.lineTo(QPointF(centerPt2.x(),centerPt2.y()-radius));
			mOval.moveTo(QPointF(centerPt1.x(),centerPt1.y()+radius));
			mOval.lineTo(QPointF(centerPt2.x(),centerPt2.y()+radius));
		}
		else
		{
			mOval.moveTo(QPointF(centerPt1.x()-radius,centerPt1.y()));
			mOval.lineTo(QPointF(centerPt2.x()-radius,centerPt2.y()));
			mOval.moveTo(QPointF(centerPt1.x()+radius,centerPt1.y()));
			mOval.lineTo(QPointF(centerPt2.x()+radius,centerPt2.y()));
		}
		mOval.connectPath(circle(centerPt1,radius));
		mOval.connectPath(circle(centerPt2,radius));
	}
	return mOval;
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

/**
  * @brief gEDA export method.
  * @return Translated gEDA object class name.
  */
QString CPcbPath::gedaObjectClass()
{
    return "ElementLine";
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA object properties.
  */
QStringList CPcbPath::gedaProperties()
{
    QStringList rc;
    if ( properties().count() >= 6 )
    {
        rc << properties().at(2);   /* X1 */
        rc << properties().at(3);   /* X2 */
        rc << properties().at(4);   /* X3 */
        rc << properties().at(5);   /* X4 */
        rc << properties().at(1);   /* Thickness */
    }
    return rc;
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA child object list.
  */
QList<CSpecctraObject*>	CPcbPath::gedaChildren()
{
    QList<CSpecctraObject*> rc;
    return rc;
}
