/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbclearanceclass.h"

#define inherited CSpecctraObject

CPcbClearanceClass::CPcbClearanceClass(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbClearanceClass::~CPcbClearanceClass()
{
}

QString CPcbClearanceClass::data()
{
	return properties()[0];
}
