/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cpcblayer.h"

#include <QDataStream>
#include <QByteArray>

#define inherited CSpecctraObject

CPcbLayer::CPcbLayer(QGraphicsItem *parent)
: inherited(parent)
{
	setColor(QColor(0xA0,0,0));
}

CPcbLayer::~CPcbLayer()
{
}

/**
  * @return the layer name
  */
QString CPcbLayer::name()
{
	return properties().at(0);
}

/**
  * @return the later type, signal, etc...
  */
QString CPcbLayer::type()
{
	CSpecctraObject* t = child("type");
	if ( t!=NULL )
	{
		if ( t->properties().count() )
		{
			return t->properties().at(0);
		}
	}
	return "";
}

/**
  * @brief @return the layer index...
  */
int CPcbLayer::index()
{
	CSpecctraObject* i = child("property");
	if ( i!=NULL )
	{
		CSpecctraObject* t = i->child("index");
		if ( t->properties().count() )
		{
			return t->properties().at(0).toInt();
		}
	}
	return 0;
}

/**
  * @return detailed descriptionm string
  */
QString CPcbLayer::description()
{
	QString rc;
	rc += "["+QString::number(index())+"]";
	rc += name();
	rc += " ("+type()+")";
	return rc;
}

void CPcbLayer::fromBytes(QByteArray data)
{
	QDataStream in(&data,QIODevice::ReadOnly);
	in >> mColor;
}

QByteArray CPcbLayer::toBytes()
{
	QByteArray data;
	QDataStream out(&data,QIODevice::WriteOnly);
	out << color();
	return data;
}


