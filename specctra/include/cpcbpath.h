/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBPATH_H
#define CPCBPATH_H

#include <QObject>
#include <QString>
#include <QPainterPath>

#include "cspecctraobject.h"

class CPcbPath : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbPath(QGraphicsItem *parent = 0);
		virtual ~CPcbPath();

        QString                         layer();
        double                          width();
        QPainterPath                    shape();
        QPainterPath                    oval();

        /* gEDA export methods */
        virtual QString					gedaObjectClass();
        virtual QStringList				gedaProperties();
        virtual QList<CSpecctraObject*>	gedaChildren();

private:
		QPainterPath			circle(QPointF center,double radius);
		QPainterPath			mOval;
};

#endif // CPCBPATH_H

