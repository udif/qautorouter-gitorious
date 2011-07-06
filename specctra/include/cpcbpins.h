/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBPINS_H
#define CPCBPINS_H

#include "cspecctraobject.h"
#include "cpcbpin.h"
#include "cpcbplace.h"
#include "cgpadstack.h"

#include <QObject>
#include <QString>
#include <QPainterPath>
#include <QList>

class CPcbPins : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbPins(QGraphicsItem *parent = 0);
		virtual ~CPcbPins();

		int							pinRefs();
		QString						pinRef(int idx);

};

#endif // CPCBPINS_H

