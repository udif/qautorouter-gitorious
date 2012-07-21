/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbpin.h"
#include "cpcbpadstack.h"
#include "cpcb.h"
#include "cpcblibrary.h"
#include "cpcbshape.h"
#include "cpcbboundary.h"
#include "cpcbrule.h"
#include "cpcbstructure.h"

#include <QList>

#define inherited CSpecctraObject

CPcbPin::CPcbPin(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbPin::~CPcbPin()
{
}

/**
  * @return the pin undex (pin number)
  */
QString CPcbPin::name()
{
	QString idx;
	if ( properties().count() >= 2 )
	{
		idx = properties().at(1);
	}
	return idx;
}

/**
  * @return the coordinates relative to the image center.
  */
QPointF CPcbPin::pos()
{
	QPointF pt(properties().at(2).toDouble(),properties().at(3).toDouble());
	return pt;
}

/**
  @ @return the name of the padtsack image
  */
QString CPcbPin::padstackName()
{
	return properties().at(0);
}

/**
  * @return a pointer to the pastack object.
  */
CPcbPadstack* CPcbPin::padstack()
{
	CPcbPadstack* ps = NULL;
	if ( pcb() != NULL && pcb()->library() != NULL )
	{
		ps = pcb()->library()->padstack(padstackName());
	}
	return ps;
}

/**
  * @brief return the clearance class
  */
CPcbClearanceClass* CPcbPin::clearanceClass()
{
	return (CPcbClearanceClass*)child("clearance_clsss");
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA object class name.
  */
QString CPcbPin::gedaObjectClass()
{
    return "Pad";
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA object properties.
  */
QStringList CPcbPin::gedaProperties()
{
    QStringList rc;
    if ( properties().count() >= 4 && padstack() != NULL )
    {
        rc << properties().at(2);                                           /* rX1 */
        rc << properties().at(3);                                           /* rX2 */
        rc << QString::number(padstack()->boundingRect().bottomRight().x());                 /* rX2 */
        rc << QString::number(padstack()->boundingRect().bottomRight().y());                 /* rY2 */
        rc << QString::number(padstack()->boundingRect().width());                           /* Thickmness */
        rc << QString::number(pcb()->structure()->rule()->clearanceOf(properties().at(0)));  /* Clearance */
        rc << /* Mask */
        rc << properties().at(0);                                           /* Name */

    }
    return rc;
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA child object list.
  */
QList<CSpecctraObject*>	CPcbPin::gedaChildren()
{
    QList<CSpecctraObject*> rc;
    return rc;
}

