/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbpins.h"
#include "cpcbplace.h"
#include "cpcbplacement.h"
#include "cpcb.h"
#include "cpcbshape.h"

#include <QStringList>
#define inherited CSpecctraObject

CPcbPins::CPcbPins(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbPins::~CPcbPins()
{
}

int CPcbPins::pinRefs()
{
	return properties().count();
}

QString CPcbPins::pinRef(int idx)
{
	QString rc;
	if ( idx < properties().count() )
	{
		rc = properties().at(idx);
	}
	return rc;
}

