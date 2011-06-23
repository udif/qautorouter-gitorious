/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCB_H
#define CPCB_H

#include <QObject>
#include "cspecctraobject.h"

class CPcbStructure;
class CPcbPlacement;
class CPcbLibrary;
class CPcb : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcb(QGraphicsItem *parent = 0);
		virtual ~CPcb();

		CPcbStructure*				structure();
		CPcbPlacement*				placement();
		CPcbLibrary*				library();
};

#endif // CPCB_H

