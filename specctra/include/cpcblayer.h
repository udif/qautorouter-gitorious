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

class CPcbType;
class CPcbLayer : public CSpecctraObject
{
	Q_OBJECT
	public:

		typedef enum
		{
			Horizontal,
			Vertical
		} tDirection;

		CPcbLayer(QGraphicsItem *parent = 0);
		virtual ~CPcbLayer();

		virtual QString					name();
		virtual QString					description();

		QColor							color() {return mColor;}
		CPcbType*						type();
		int								index();
		void							setDirection(tDirection direction) {mDirection = direction;}
		tDirection						direction() {return mDirection;}

		void							setColor(QColor color) {mColor=color;}

		void							fromBytes(QByteArray data);
		QByteArray						toBytes();
	private:
		QColor							mColor;
		tDirection						mDirection;
};

#endif // CPCBLAY_H

