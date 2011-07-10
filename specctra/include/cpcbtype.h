/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBTYPE_H
#define CPCBTYPE_H

#include <QObject>
#include <QString>
#include <QRectF>
#include <QPainterPath>

#include "cspecctraobject.h"

class CPcbType : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbType(QGraphicsItem *parent = 0);
		virtual ~CPcbType();

		void					setData(QString w);
		QString					data();
};

#endif // CPCBTYPE_H

