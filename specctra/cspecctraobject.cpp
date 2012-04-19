/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cspecctraobject.h"
#include <stdio.h>

#define inherited QGraphicsItem

QGraphicsScene*									CSpecctraObject::mGlobalScene=NULL;	/** The graphics scheen */
QMap<CSpecctraObject::tDrawableClass,double>	CSpecctraObject::mOpacity;			/** Object opacity map by drawable class */
QMap<CSpecctraObject::tDrawableClass,bool>		CSpecctraObject::mVisibility;		/** Object visibility map by drawable class */

CSpecctraObject::CSpecctraObject(QGraphicsItem* parent)
: inherited(parent)
, mParentObject(NULL)
{
	bool v = mVisibility[drawableClass()];
	double o = mOpacity[drawableClass()];
	setOpacity(o);
	setVisible(v);
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

/**
  * @brief copy from another.
  */
CSpecctraObject& CSpecctraObject::copy( const CSpecctraObject& other )
{
	CSpecctraObject* pOther = (CSpecctraObject*)&other;
	mObjectClass = pOther->objectClass();
	mProperties = pOther->properties();
	mChildren = pOther->children();
	mParentObject = pOther->parentObject();
	return *this;
}

/**
  * @return the name of the object, normally the specctra class name
  */
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

/**
  * @return a reference to the list of object properties as defined by the specctra file.
  */
QStringList& CSpecctraObject::CSpecctraObject::properties()
{
	return mProperties;
}

/**
  * @return a reference to the list of child objects.
  */
QList<CSpecctraObject*>& CSpecctraObject::children()
{
	return mChildren;
}

/**
  * @return the drawable class for this type of object
  */
CSpecctraObject::tDrawableClass CSpecctraObject::drawableClass()
{
	return CSpecctraObject::None;
}

/**
  * @brief For each object of the specified drawable class, set the opacity value.
  * @param drawableClass The class of drawable object, Track, Via, etc...
  * @param opacity The value of opacity to apply to the class of objects.
  */
void CSpecctraObject::setClassOpacity(tDrawableClass dClass,double opacity)
{
	if ( opacity > 1.0 ) opacity=1.0;
	if ( opacity < 0.0 ) opacity-0.0;
	if ( drawableClass() == dClass )
	{
		this->setOpacity(opacity);
	}
	for(int n=0; n < children().count(); n++)
	{
		CSpecctraObject* child = children()[n];
		child->setClassOpacity(dClass,opacity);
	}
}

/**
  * @brief For each object of the specified drawable class, set the visibility flag.
  * @param drawableClass The class of drawable object, Track, Via, etc...
  * @param visibility boolean.
  */
void CSpecctraObject::setClassVisibility(tDrawableClass dClass,bool visible)
{
	if ( drawableClass() == dClass )
	{
		this->setVisible(visible);
	}
	for(int n=0; n < children().count(); n++)
	{
		CSpecctraObject* child = children()[n];
		child->setClassVisibility(dClass,visible);
	}
}

/**
  * @brief used for streaming to a text stream. This shoule render ieself as a specctra style output.
  */
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

/**
  * @return All child objects of objectClass
  */
QList<CSpecctraObject*> CSpecctraObject::collect(QString oc)
{
	QList<CSpecctraObject*> rc;
	/* Am I one of them? */
	if (this->objectClass() == oc)
	{
		rc += this;
	}
	/* Are any of my descendants one of them? */
	for(int n=0; n < children().count(); n++)
	{
		rc += children().at(n)->collect(oc);
	}
	return rc;
}


/**
  * @return the root of the specctra object tree, should normally be a "pcb" object.
  */
CSpecctraObject* CSpecctraObject::root()
{
	if ( parentObject() != NULL )
		return parentObject()->root();
	return this;
}

/**
  * @return the pcb object, should normally be the root object.
  */
CPcb* CSpecctraObject::pcb()
{
	if ( root() != NULL && root()->objectClass() == "pcb" )
		return (CPcb*)root();
	return NULL;
}

/**
  * @brief Currently only one scene/view is supported, this is a global static.
  */
QGraphicsScene* CSpecctraObject::globalScene()
{
	if ( mGlobalScene == NULL )
		mGlobalScene = new QGraphicsScene;
	return mGlobalScene;
}

/**
  * @brief Find a parent or great...grand parent object of the specified class.
  * @return a pointer to the parent object or NULL if no such object exists.
  */
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

/**
  * @brief used for debugging, dumps the contents of the object to the console.
  */
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

/**
  * @brief search for a rule object belonging to this object.
  * @return NULL if none is found.
  */
CPcbRule* CSpecctraObject::rule()
{
	return (CPcbRule*)child("rule");
}

/**
  * @brief Generally gets overridden by drawable objects, erturns a bounding rectangle of the drawable area.
  */
QRectF CSpecctraObject::boundingRect() const
{
	QRectF r;
	return r;
}

/**
  * @brief Generally is overridden by drawable objects, and performs the actual rendering
  */
void CSpecctraObject::paint(QPainter*,const QStyleOptionGraphicsItem*, QWidget*)
{
}

