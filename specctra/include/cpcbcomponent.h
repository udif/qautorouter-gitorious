/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBCOMPONENT_H
#define CPCBCOMPONENT_H

#include <QObject>
#include <QString>

#include "cspecctraobject.h"

class CPcbPlace;
class CPcbComponent : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbComponent(QGraphicsItem *parent = 0);
		virtual ~CPcbComponent();

		QString							footprint();
		int								places();
		CPcbPlace*						place(int idx);
};

#endif // CPCBCOMPONENT_H

