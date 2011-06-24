/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbvia.h"
#include "cpcb.h"
#include "cpcblibrary.h"
#include "cpcbpadstack.h"

#define inherited CSpecctraObject

CPcbVia::CPcbVia(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbVia::~CPcbVia()
{
}

/**
  * @return a pointer to the padstack object
  */
CPcbPadstack* CPcbVia::padstack()
{
	CPcbPadstack* ps=NULL;
	if ( pcb() != NULL && pcb()->library() != NULL && properties().count() )
	{
		ps = pcb()->library()->padstack(properties().at(0));
	}
	return ps;
}
