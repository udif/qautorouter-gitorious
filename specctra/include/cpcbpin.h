/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBPIN_H
#define CPCBPIN_H

#include <QObject>
#include <QString>
#include <QPointF>

#include "cspecctraobject.h"

class CPcbClearanceClass;
class CPcbPadstack;
class CPcbPin : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbPin(QGraphicsItem *parent = 0);
		virtual ~CPcbPin();

		virtual QString			name();
		QPointF					pos();
		QString					padstackName();
		CPcbPadstack*			padstack();
		CPcbClearanceClass*		clearanceClass();

        /* gEDA export methods */
        virtual QString					gedaObjectClass();
        virtual QStringList				gedaProperties();
        virtual QList<CSpecctraObject*>	gedaChildren();
};

#endif // CPCBPIN_H

