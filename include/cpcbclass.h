/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBCLASS_H
#define CPCBCLASS_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "cspecctraobject.h"

class CPcbNet;
class CPcbRule;
class CPcbClass : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbClass(QGraphicsItem *parent = 0);
		virtual ~CPcbClass();

		virtual QString					name();
		virtual QString					description();

		QStringList&					nets();

		double							width();

	private:
		QStringList						mNets;

};

#endif // CPCBCLASS_H

