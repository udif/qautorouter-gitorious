/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcbplace.h"
#include "cpcbcomponent.h"
#include "cpcblibrary.h"
#include "cpcb.h"
#include "cpcboutline.h"
#include "cpcbpin.h"
#include "cpcbimage.h"
#include "cpcbpath.h"
#include "cpcbpadstack.h"
#include "cpcbshape.h"

#include "cgpadstack.h"
#include "cgpad.h"
#include "cutil.h"

#define inherited CSpecctraObject

CPcbPlace::CPcbPlace(QGraphicsItem *parent)
: inherited(parent)
, mPadstacksCreated(false)
{
	CSpecctraObject::globalScene()->addItem(this);
}

CPcbPlace::~CPcbPlace()
{
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA object class name.
  */
QString CPcbPlace::gedaObjectClass()
{
    return "Element";
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA object properties.
  */
QStringList CPcbPlace::gedaProperties()
{
    QStringList rc;
    if ( properties().count() >= 5 )
    {
        CPcbComponent* component = (CPcbComponent*)parentObject("component");
        CSpecctraObject* pn = child("pn");
        if ( component != NULL && pn != NULL )
        {
            rc << "0";                                          /* SFlag */
            rc << "\""+component->properties().at(0)+"\"";      /* Desc */
            rc << "\""+properties().at(0)+"\"";                 /* Name */
            rc << "\""+pn->properties().at(0)+"\"";             /* Value */
            rc << properties().at(1);                           /* MX */
            rc << properties().at(2);                           /* MY */
            rc << properties().at(1);                           /* TX */
            rc << properties().at(2);                           /* TY */
            rc << properties().at(4);                           /* TDir */
            rc << "100";                                        /* TScale */
            rc << "0";                                          /* TSFlags */
        }
    }
    return rc;
}

/**
  * @brief gEDA export method.
  * @return Translated gEDA child object list.
  */
QList<CSpecctraObject*>	CPcbPlace::gedaChildren()
{
    QList<CSpecctraObject*> rc;
    CPcbLibrary* library = (CPcbLibrary*)pcb()->library();
    CPcbComponent* component = (CPcbComponent*)parentObject("component");
    if ( library != NULL && component != NULL )
    {
        QString imageName = component->properties().at(0); /* Desc */
        CPcbImage* image = library->image(imageName);
        if ( image != NULL )
        {
            /* FIXME Since gEDA instatiates an instance of image for each placement, we have to do the
              coordinate translation somewhere here
            */
            QList<CSpecctraObject*>& imageChildren = image->children();
            for(int n=0; n < imageChildren.count(); n++)
            {
                CSpecctraObject* child = imageChildren.at(n);
                /* extract the paths from the outlines.. */
                if ( child->objectClass() == "outline" )
                {
                    CPcbPath* path = (CPcbPath*)child->child("path");
                    if ( path != NULL )
                    {
                        rc.append( path );
                    }
                }
                else
                {
                    rc.append( child );
                }
            }
        }
    }
    return rc;
}

/**
  * @return the class of drawable object which this is.
  */
CSpecctraObject::tDrawableClass CPcbPlace::drawableClass()
{
	return CSpecctraObject::Outline;
}

/**
  * @return the unit identifier.
  */
QString CPcbPlace::unit()
{
	QString uid;
	if ( properties().count() )
	{
		uid = properties().at(0);
	}
	return uid;
}

/**
  * @return the position point.
  */
QPointF CPcbPlace::pos()
{
	QPointF pt;
	if ( properties().count() )
	{
		pt.setX(properties().at(1).toDouble());
		pt.setY(properties().at(2).toDouble());
	}
	return pt;

}

/**
  * @return which side of the pcb.
  */
QString CPcbPlace::side()
{
	QString sd;
	if ( properties().count() )
	{
		sd = properties().at(3);
	}
	return sd;
}

/**
  * @return the number of pads
  */
int CPcbPlace::pads()
{
	return mPads.count();
}

/**
  * @return a pad by index
  */
CGPadstack* CPcbPlace::pad(int idx)
{
	if ( idx < mPads.count() )
	{
		return mPads.at(idx);
	}
	return NULL;
}

/**
  * @return a pad by PIN string reference.
  */
CGPadstack* CPcbPlace::pad(QString ref)
{
	for( int n=0; n < mPads.count(); n++)
	{
		CGPadstack* padstack = mPads.at(n);
		if ( padstack->pinRef() == ref )
		{
			return padstack;
		}
	}
	return NULL;
}


/**
  * @return a pin pointer by it's index name
  */
CPcbPin* CPcbPlace::pin(QString ref)
{
	CSpecctraObject* pObj = parentObject();
	if ( pObj->objectClass() == "component")
	{
		CPcbComponent* component = (CPcbComponent*)pObj;
		CPcbLibrary* library = pcb()->library();
		if ( library != NULL )
		{
			CPcbImage* image = library->image(component->footprint());
			CPcbPin* pin = image->pin(ref);
			if ( pin != NULL )
				return pin;
		}
	}
	return NULL;
}

/**
  * @return the rotation angle.
  */
double CPcbPlace::rotation()
{
	int rot;
	if ( properties().count() )
	{
		rot = properties().at(4).toDouble();
	}
	return rot;
}

QRectF CPcbPlace::boundingRect() const
{
	QRectF bounds = shape().boundingRect();
	return bounds;
}

QPainterPath& CPcbPlace::outlineShape()
{
	if ( mOutlineShape.isEmpty() )
	{
		CPcbPlace* me = (CPcbPlace*)this;
		if ( me->pcb() != NULL )
		{
			QPainterPath p;
			CSpecctraObject* pObj = me->parentObject();
			if ( pObj->objectClass() == "component")
			{
				CPcbComponent* component = (CPcbComponent*)pObj;
				CPcbLibrary* library = me->pcb()->library();
				if ( library != NULL )
				{
					QPainterPath outlineShape;
					CPcbImage* image = library->image(component->footprint());
					int nOutlines = image->outlines();
					int iOutline;
					for(iOutline=0;iOutline<nOutlines;iOutline++)
					{
						CPcbOutline* outline = image->outline(iOutline);
						if ( outline!=NULL && outline->path()!=NULL )
						{
							outlineShape.addPath(outline->path()->shape());
						}
					}
					outlineShape.translate(me->pos());
					p.addPath(outlineShape);
				}
			}
			me->setTransformOriginPoint(me->pos());
			me->setRotation(me->rotation());
			me->mOutlineShape =  p;
			me->createPadstacks();
		}
	}
	return mOutlineShape;
}

/**
  * @brief create the graphical padstack instances for each pni reference.
  */
void CPcbPlace::createPadstacks()
{
	if ( !mPadstacksCreated )
	{
		CSpecctraObject* pObj = parentObject();
		if ( pObj->objectClass() == "component" )
		{
			CPcbComponent* component = (CPcbComponent*)pObj;
			CPcbLibrary* library = pcb()->library();
			if ( library != NULL )
			{
				CPcbImage* image = library->image(component->footprint());
				for(int iPin=0;iPin<image->pins();iPin++)
				{
					QPainterPath pinShape;
					CPcbPin* pin = image->pin(iPin);
					CPcbPadstack* padstack = pin->padstack();
					CGPadstack* cgPadstack = new CGPadstack((CPcbNet*)NULL,this,pin->name());
					mPads.append(cgPadstack);
					for(int iShape=0;iShape<padstack->shapes();iShape++)
					{
						CPcbShape* shape = padstack->shape(iShape);
						QPainterPath shapeShape = shape->shape();
						shapeShape.translate(pin->pos());
						shapeShape.translate(pos());
						CGPad* pad = new CGPad(cgPadstack,shapeShape,shape->layer());
						pad->setTransformOriginPoint(pos());
						pad->setRotation(rotation());
					}
				}
				mPadstacksCreated = true;
			}
		}
	}
}

QPainterPath CPcbPlace::shape() const
{
	CPcbPlace* me = (CPcbPlace*)this;
	QPainterPath p = me->outlineShape();
	return p;
}

void CPcbPlace::paint(QPainter *painter, const QStyleOptionGraphicsItem* /* option */, QWidget* /* widget */)
{
	painter->setRenderHint(QPainter::Antialiasing);
	painter->scale(scale(),scale());
	painter->setPen(QPen(QColor(0,255,255), 10, Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
	painter->drawPath(outlineShape());

}
