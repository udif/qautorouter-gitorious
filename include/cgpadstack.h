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

#include "cgsegment.h"
#include "cgpad.h"

class CPcb;
class CPcbPlace;
class CGPadstack : public CGSegment
{
	Q_OBJECT
	public:
		CGPadstack(CPcbPlace* place,QString pinRef,QObject *parent = 0);
		virtual ~CGPadstack();

		virtual tSegment			segmentType() {return CGSegment::Padstack;}
		virtual bool				isA(CGSegment::tSegment t) {return t==CGSegment::Padstack || CGSegment::isA(t);}

		void						addPad(CGPad* pad);
		CPcbPlace*					place();
		CPcb*						pcb();
		CGPad*						pad(QString layer);
		QStringList					layers();
		QString						pinRef();
		QString						unitRef();
		virtual QPointF				origin();

		static void					erase();
		static CGPadstack*			padstack(QString unitRef);
		static QMap<QString,CGPadstack*> padstacks() {return mGPadstacks;}

		virtual QPainterPath		shape() const;

	private:
		static QMap<QString,CGPadstack*> mGPadstacks;

		CPcbPlace*					mPlace;
		QString						mPinRef;
		QMap<QString,CGPad*>		mPads;

};

#endif // CGPADSTACK_H
