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

class CPcbPadstack;
class CPcbPin : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbPin(QGraphicsItem *parent = 0);
		virtual ~CPcbPin();

		QString					index();
		QPointF					pos();
		QString					padstackName();
		CPcbPadstack*			padstack();

};

#endif // CPCBPIN_H

