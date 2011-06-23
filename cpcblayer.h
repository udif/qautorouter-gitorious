/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCLAYER_H
#define CPCLAYER_H

#include <QObject>
#include <QString>
#include "cspecctraobject.h"

class CPcbLayer : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbLayer(QGraphicsItem *parent = 0);
		virtual ~CPcbLayer();

		QString							type();
		int								index();

};

#endif // CPCBLAY_H

