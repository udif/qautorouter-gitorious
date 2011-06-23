/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCLAYER_H
#define CPCLAYER_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QColor>

#include "cspecctraobject.h"

class CPcbLayer : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbLayer(QGraphicsItem *parent = 0);
		virtual ~CPcbLayer();

		QColor							color() {return mColor;}
		QString							name();
		QString							type();
		int								index();
		QString							description();

		void							setColor(QColor color) {mColor=color;}

		void							fromBytes(QByteArray data);
		QByteArray						toBytes();
	private:
		QColor							mColor;
};

#endif // CPCBLAY_H

