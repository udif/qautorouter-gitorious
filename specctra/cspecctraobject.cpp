/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cspecctraobject.h"
#include <stdio.h>

//#define inherited QGraphicsPathItem
#define inherited QGraphicsItem

QGraphicsScene* CSpecctraObject::mGlobalScene=NULL;					/** The graphics scheen */

CSpecctraObject::CSpecctraObject(QGraphicsItem* parent)
: inherited(parent)
, mParentObject(NULL)
{
	//setCacheMode(QGraphicsItem::NoCache);
}

CSpecctraObject::CSpecctraObject( const CSpecctraObject& other)
: QObject()
, inherited(NULL)
, mParentObject(NULL)
{
	copy(other);
}

CSpecctraObject::~CSpecctraObject()
{
	while(mChildren.count())
	{
		CSpecctraObject* child = mChildren.takeFirst();
		if ( child->scene() == NULL )
			delete child;
	}
	mChildren.clear();
}

QString CSpecctraObject::name()
{
	return objectClass();
}

QString CSpecctraObject::description()
{
	return name();
}

void CSpecctraObject::setParent(CSpecctraObject* p)
{
	mParentObject=p;
}

void CSpecctraObject::setObjectClass(QString& objectClass)
{
	mObjectClass = objectClass.toLower();
}

void CSpecctraObject::appendProperty(QString& property)
{
	mProperties.append(property);
}

void CSpecctraObject::appendProperty(QStringList& property)
{
	mProperties += property;
}

void CSpecctraObject::appendChild(CSpecctraObject* child)
{
	mChildren.append(child);child->setParent(this);
}

QString& CSpecctraObject::objectClass()
{
	return mObjectClass;
}

QStringList& CSpecctraObject::CSpecctraObject::properties()
{
	return mProperties;
}

QList<CSpecctraObject*>& CSpecctraObject::children()
{
	return mChildren;
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
	if ( root() != NULL && root()->objectClass() == "pcb" )
		return (CPcb*)root();
	return NULL;
}

QGraphicsScene* CSpecctraObject::globalScene()
{
	if ( mGlobalScene == NULL )
		mGlobalScene = new QGraphicsScene;
	return mGlobalScene;
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

CSpecctraObject* CSpecctraObject::parentObject(QString o)
{
	if ( mParentObject != NULL )
	{
		if ( o.isEmpty() || (!o.isEmpty() && mParentObject->objectClass() == o) )
		{
			return mParentObject;
		}
		if ( !o.isEmpty() )
		{
			return mParentObject->parentObject(o);
		}
	}
	if (!o.isEmpty() && objectClass() == o)
		return this;
	return NULL;
}

/**
  * @count the number of children of a particular object class.
  */
int CSpecctraObject::childCount(QString o)
{
	int count=0;
	for(int n=0; n < children().count(); n++)
	{
		if ( children().at(n)->objectClass()==o)
		{
			++count;
		}
	}
	return count;

}

/**
  * @return a child of class
  */
CSpecctraObject* CSpecctraObject::child(QString o,int idx)
{
	for(int n=0; n < children().count(); n++)
	{
		if ( children().at(n)->objectClass()==o)
		{
			if ( idx == 0 )
				return children().at(n);
			--idx;
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

CPcbRule* CSpecctraObject::rule()
{
	return (CPcbRule*)child("rule");
}

QRectF CSpecctraObject::boundingRect() const
{
	QRectF r;
	return r;
}

void CSpecctraObject::paint(QPainter*,const QStyleOptionGraphicsItem*, QWidget*)
{
}

