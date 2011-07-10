/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbwidth.h"

#define inherited CSpecctraObject

CPcbWidth::CPcbWidth(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbWidth::~CPcbWidth()
{
}

/**
  * @brief set the track width
  */
void CPcbWidth::setData(double w)
{
	QString wStr = QString::number(w);
	if ( properties().isEmpty() )
		properties().append(wStr);
	properties()[0]=wStr;
}

/**
  * @return the default track width
  */
double CPcbWidth::data()
{
	double w=0;
	if ( !properties().isEmpty() )
		w = properties()[0].toDouble();
	return w;
}
