/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBWIRING_H
#define CPCBWIRING_H

#include "cspecctraobject.h"

class CPcbWire;
class CPcbWiring : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbWiring(QGraphicsItem *parent = 0);
		virtual ~CPcbWiring();

		int								wires();
		CPcbWire*						wire(int idx);
};

#endif // CPCBWIRING_H

