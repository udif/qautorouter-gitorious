/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBCLASS_H
#define CPCBCLASS_H

#include <QObject>
#include <QString>

#include "cspecctraobject.h"

class CPcbNet;
class CPcbClass : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbClass(QGraphicsItem *parent = 0);
		virtual ~CPcbClass();

		QString							name();

		int								nets();
		CPcbNet*						net(int idx);

	private:
		int								nNets;
		QList<CPcbNet*>					mNets;

};

#endif // CPCBCLASS_H

