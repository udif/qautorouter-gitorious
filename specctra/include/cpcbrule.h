/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBRULE_H
#define CPCBRULE_H

#include <QObject>
#include <QString>
#include <QRectF>
#include <QPainterPath>

#include "cspecctraobject.h"

class CPcbWidth;
class CPcbClearance;
class CPcbRule : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbRule(QGraphicsItem *parent = 0);
		virtual ~CPcbRule();

		CPcbWidth*					width();

        int                         clearances();
        CPcbClearance*              clearance(int idx=0);
        double                      clearanceOf(QString type);
};

#endif // CPCBRULE_H

