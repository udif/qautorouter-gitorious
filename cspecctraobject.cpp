/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cspecctraobject.h"
#include <stdio.h>

//#define inherited QGraphicsPathItem
#define inherited QGraphicsItem

QGraphicsScene* CSpecctraObject::mScene=NULL;					/** The graphics scheen */

CSpecctraObject::CSpecctraObject(QGraphicsItem* parent)
: inherited(parent)
, mParentObject(NULL)
{
	//setCacheMode(QGraphicsItem::NoCache);
}

CSpecctraObject::CSpecctraObject( const CSpecctraObject& other,QGraphicsItem* parent)
: inherited(parent)
, mParentObject(NULL)
{
	copy(other);
}

CSpecctraObject::~CSpecctraObject()
{
	for(int n=0; n < mChildren.count(); n++)
	{
		delete mChildren.at(n);
	}
	mChildren.clear();
}

QString CSpecctraObject::toText(int lvl)
{
	QString fill;
	QString text;
	fill.fill(' ',lvl*2);
	text += fill;
	text += "("+objectClass()+" ";
	text += properties().join(" ");
	if ( children().count() )
	{
		text += "\n";
		for(int n=0;n<children().count();n++)
		{
			CSpecctraObject* child = children().at(n);
			text += child->toText(lvl+1);
		}
		text += fill;
	}
	text += ")\n";
	return text;
}

CSpecctraObject* CSpecctraObject::root()
{
	if ( parentObject() != NULL )
		return parentObject()->root();
	return this;
}

CPcb* CSpecctraObject::pcb()
{
	CSpecctraObject* obj = root();
	if (obj->objectClass()=="pcb")
	{
		return (CPcb*)obj;
	}
	return NULL;
}

QGraphicsScene* CSpecctraObject::scene()
{
	if ( mScene == NULL )
		mScene = new QGraphicsScene;
	return mScene;
}


CSpecctraObject& CSpecctraObject::copy( const CSpecctraObject& other )
{
	CSpecctraObject* pOther = (CSpecctraObject*)&other;
	mObjectClass = pOther->objectClass();
	mProperties = pOther->properties();
	mChildren = pOther->children();
	mParentObject = pOther->parentObject();
	return *this;
}

CSpecctraObject* CSpecctraObject::child(QString o)
{
	for(int n=0; n < children().count(); n++)
	{
		if ( children().at(n)->objectClass()==o)
		{
			return children().at(n);
		}
	}
	return NULL;
}

void CSpecctraObject::dump(int lvl)
{
	for(int n=0; n < lvl;n++)
		printf("+");
	printf("[%s] ", objectClass().toAscii().data());
	for(int n=0; n < properties().count(); n++)
	{
		printf("%s,",properties().at(n).toAscii().data());
	}
	printf("\n");
	for(int n=0; n < children().count(); n++)
	{
		CSpecctraObject* child = children().at(n);
		child->dump(lvl+1);
	}
}
