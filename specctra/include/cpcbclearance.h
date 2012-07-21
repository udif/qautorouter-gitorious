/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBCLEARANCE_H
#define CPCBCLEARANCE_H

#include <QObject>
#include <QString>
#include <QRectF>
#include <QPainterPath>

#include "cspecctraobject.h"

class CPcbClearance : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbClearance(QGraphicsItem *parent = 0);
		virtual ~CPcbClearance();

		void					setData(double w);
		double					data();

        QString                 type();
};

#endif // CPCBCLEARANCE_H

