/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBCLEARANCECLASS_H
#define CPCBCLEARANCECLASS_H

#include "cspecctraobject.h"

#include <QPointF>
#include <QString>

class CPcbClearanceClass : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbClearanceClass(QGraphicsItem *parent = 0);
		virtual ~CPcbClearanceClass();

		QString							data();

};

#endif // CPCBCLEARANCECLASS_H

