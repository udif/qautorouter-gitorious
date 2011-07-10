/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBWIDTH_H
#define CPCBWIDTH_H

#include <QObject>
#include <QString>
#include <QRectF>
#include <QPainterPath>

#include "cspecctraobject.h"

class CPcbWidth : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbWidth(QGraphicsItem *parent = 0);
		virtual ~CPcbWidth();

		void					setData(double w);
		double					data();
};

#endif // CPCBWIDTH_H

