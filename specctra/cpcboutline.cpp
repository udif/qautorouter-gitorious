/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcboutline.h"
#include "cpcbimage.h"
#include "cpcbpath.h"

#define inherited CSpecctraObject

CPcbOutline::CPcbOutline(QGraphicsItem *parent)
: inherited(parent)
{
}

CPcbOutline::~CPcbOutline()
{
}
/**
  * @return the component count
  */
int CPcbOutline::paths()
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "path" )
		{
			++count;
		}
	}
	return count;
}

/**
  * @return a component by index
  */
CPcbPath* CPcbOutline::path(int comp)
{
	int count=0;
	for(int n=0;n<children().count();n++)
	{
		if ( children().at(n)->objectClass() == "path" )
		{
			if ( count == comp )
			{
				return (CPcbPath*)children().at(n);
			}
			++count;
		}
	}
	return NULL;
}



