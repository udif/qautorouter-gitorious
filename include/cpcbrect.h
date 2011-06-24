/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBRECT_H
#define CPCBRECT_H

#include <QObject>
#include <QString>
#include <QRectF>
#include <QPainterPath>

#include "cspecctraobject.h"

class CPcbRect : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbRect(QGraphicsItem *parent = 0);
		virtual ~CPcbRect();

		QRectF			rect();
		QString			layer();
		QPainterPath	path();
};

#endif // CPCBRECT_H

