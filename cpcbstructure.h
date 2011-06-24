/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCSTRUCTUREB_H
#define CPCSTRUCTUREB_H

#include <QObject>
#include <QList>
#include "cspecctraobject.h"

class CPcbBoundary;
class CPcbLayer;
class CPcbRule;
class CPcbStructure : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbStructure(QGraphicsItem *parent = 0);
		virtual ~CPcbStructure();

		int								layers();
		CPcbLayer*						layer(int idx);
		CPcbLayer*						layer(QString ref);

		CPcbRule*						rule();

		CPcbBoundary*					boundary();
};

#endif // CPCSTRUCTUREB_H

