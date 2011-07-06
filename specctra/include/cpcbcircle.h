/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBCIRCLE_H
#define CPCBCIRCLE_H

#include <QObject>
#include <QString>
#include <QPainterPath>

#include "cspecctraobject.h"

class CPcbCircle : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbCircle(QGraphicsItem *parent = 0);
		virtual ~CPcbCircle();

		double			radius();
		QString			layer();
		QPainterPath	path();
};

#endif // CPCBCIRCLE_H

