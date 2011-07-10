/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbpin.h"
#include "cpcbpadstack.h"
#include "cpcb.h"
#include "cpcblibrary.h"

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

