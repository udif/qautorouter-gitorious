/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCPLACEMENT_H
#define CPCPLACEMENT_H

#include <QObject>
#include <QString>
#include <QPolygonF>
#include <QList>

#include "cspecctraobject.h"

class CPcbComponent;
class CPcbPlace;
class CPcbPlacement : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbPlacement(QGraphicsItem *parent = 0);
		virtual ~CPcbPlacement();

		int								components();
		CPcbComponent*					component(int idx);
		CPcbComponent*					component(QString footprint);

		int								places();
		CPcbPlace*						place(int idx);
		CPcbPlace*						place(QString uid);

	public slots:
		virtual void					clearCache();

	private:
		QList<CPcbComponent*>			mComponents;
		QList<CPcbPlace*>				mPlaces;
};

#endif // CPCPLACEMENT_H

