/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBSHAPE_H
#define CPCBSHAPE_H

#include "cspecctraobject.h"

#include <QObject>
#include <QString>
#include <QPolygonF>
#include <QStringList>
#include <QPainterPath>

class CPcbShape : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbShape(QGraphicsItem *parent = 0);
		virtual ~CPcbShape();

		QString				layer();
		QPainterPath		shape();
};

#endif // CPCBSHAPE_H

