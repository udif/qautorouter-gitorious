/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBNETWORK_H
#define CPCBNETWORK_H

#include <QObject>
#include <QString>

#include "cspecctraobject.h"

class CPcbNet;
class CPcbClass;
class CPcbNetwork : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbNetwork(QGraphicsItem *parent = 0);
		virtual ~CPcbNetwork();

		int								nets();
		CPcbNet*						net(int idx);
		CPcbNet*						net(QString ref);
		QList<CPcbNet*>&				netsRef();


		int								netClasses();
		CPcbClass*						netClass(int idx);
		CPcbClass*						netClass(QString ref);

		int								routed();

	private:
		QList<CPcbNet*>					mNets;
		QList<CPcbClass*>				mClasses;

};

#endif // CPCBNETWORK_H

