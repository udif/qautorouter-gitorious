/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbtype.h"

#define inherited CSpecctraObject

CPcbType::CPcbType(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbType::~CPcbType()
{
}

/**
  * @brief set the track width
  */
void CPcbType::setData(QString w)
{
	if ( properties().isEmpty() )
		properties().append(w);
	properties()[0]=w;
}

/**
  * @return the default track width
  */
QString CPcbType::data()
{
	QString w;
	if ( !properties().isEmpty() )
		w = properties()[0];
	return w;
}
