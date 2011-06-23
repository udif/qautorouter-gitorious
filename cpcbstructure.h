/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCSTRUCTUREB_H
#define CPCSTRUCTUREB_H

#include <QObject>
#include <QList>
#include "cspecctraobject.h"
#include "cpcblayer.h"

class CPcbBoundary;
class CPcbStructure : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbStructure(QGraphicsItem *parent = 0);
		virtual ~CPcbStructure();

		virtual void					appendChild(CSpecctraObject* child);

		QList<CPcbLayer*>&				layers() {return mLayers;}
		CPcbBoundary*					boundary();

	private:
		QList<CPcbLayer*>				mLayers;			/* pcb layers */
};

#endif // CPCSTRUCTUREB_H

