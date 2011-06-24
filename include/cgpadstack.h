/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGPADSTACK_H
#define CGPADSTACK_H

#include <QObject>
#include <QPainterPath>
#include <QString>
#include <QGraphicsItem>
#include <QMap>

#include "cgpad.h"

class CPcb;
class CPcbPlace;
class CGPadstack : public QObject
{
	Q_OBJECT
	public:
		CGPadstack(CPcbPlace* place,QString pinRef,QObject *parent = 0);
		virtual ~CGPadstack();

		void						addPad(CGPad* pad);
		CPcbPlace*					place();
		CPcb*						pcb();
		CGPad*						pad(QString layer);
		QStringList					layers();
		QString						pinRef();
		QString						unitRef();
		QPointF						pos();

		static void					clear();
		static CGPadstack*			padstack(QString unitRef);
		static QMap<QString,CGPadstack*> padstacks() {return mGPadstacks;}

	private:
		static QMap<QString,CGPadstack*> mGPadstacks;

		CPcbPlace*					mPlace;
		QString						mPinRef;
		QMap<QString,CGPad*>		mPads;

};

#endif // CGPADSTACK_H
