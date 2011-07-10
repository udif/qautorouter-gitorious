/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbclearance.h"

#define inherited CSpecctraObject

CPcbClearance::CPcbClearance(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbClearance::~CPcbClearance()
{
}

/**
  * @brief set the track width
  */
void CPcbClearance::setData(double w)
{
	QString wStr = QString::number(w);
	if ( properties().isEmpty() )
		properties().append(wStr);
	properties()[0]=wStr;
}

/**
  * @return the default track width
  */
double CPcbClearance::data()
{
	double w=0;
	if ( !properties().isEmpty() )
		w = properties()[0].toDouble();
	return w;
}
