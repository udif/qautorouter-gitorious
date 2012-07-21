/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcbplacement.h"
#include "cpcblibrary.h"
#include "cpcbnetwork.h"
#include "cpcbboundary.h"

#include <QEventLoop>

#define inherited CSpecctraObject

CPcb::CPcb(QGraphicsItem *parent)
: inherited(parent)
, mStructure(NULL)
, mPlacement(NULL)
, mLibrary(NULL)
, mNetwork(NULL)

{
}

CPcb::~CPcb()
{
}

/**
  * @brief provide a hook for plugin to yield...
  */
void CPcb::yield()
{
	QEventLoop loop;
	loop.processEvents();
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA object class name.
  */
QString CPcb::gedaObjectClass()
{
    return "PCB";
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA object properties.
  */
QStringList CPcb::gedaProperties()
{
    QStringList rc;
    if ( properties().count() )
    {
        CPcbStructure* structure = (CPcbStructure*)child("structure");
        if ( structure != NULL )
        {
            CPcbBoundary* boundary = structure->boundary();
            if ( boundary != NULL )
            {
                rc << "\"" + properties().at(0) + "\"";                     /* Name */
                rc << QString::number(boundary->boundingRect().width());    /* Width */
                rc << QString::number(boundary->boundingRect().height());   /* Height */
            }
        }
    }
    return rc;
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA child object list.
  */
QList<CSpecctraObject*>	CPcb::gedaChildren()
{
    QList<CSpecctraObject*> rc;
    return rc;
}

/**
  * @return the pcb structure object
  */
CPcbStructure* CPcb::structure()
{
	if ( mStructure == NULL )
	{
		mStructure = (CPcbStructure*)child("structure");
	}
	return mStructure;
}

/**
  * @return the pcb placement object
  */
CPcbPlacement* CPcb::placement()
{
	if ( mPlacement == NULL )
	{
		mPlacement = (CPcbPlacement*)child("placement");
	}
	return mPlacement;
}

/**
  * @return the pcb library object
  */
CPcbLibrary* CPcb::library()
{
	if ( mLibrary == NULL )
	{
		mLibrary = (CPcbLibrary*)child("library");
	}
	return mLibrary;
}

/**
  * @return the pcb network object
  */
CPcbNetwork* CPcb::network()
{
	if ( mNetwork == NULL )
	{
		mNetwork = (CPcbNetwork*)child("network");
	}
	return mNetwork;
}

