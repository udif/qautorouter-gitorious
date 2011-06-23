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
class CPcbNetwork : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbNetwork(QGraphicsItem *parent = 0);
		virtual ~CPcbNetwork();

		int								nets();
		CPcbNet*						net(int idx);
		CPcbNet*						net(QString name);

	private:
		int								nNets;
		QList<CPcbNet*>					mNets;

};

#endif // CPCBNETWORK_H

