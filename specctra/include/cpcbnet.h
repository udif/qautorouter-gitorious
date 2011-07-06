/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBNET_H
#define CPCBNET_H

#include "cspecctraobject.h"
#include "cutil.h"
#include "cgwire.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QPainterPath>

class CPcbPin;
class CPcbClass;
class CGPadstack;
class CGWire;
class CPcbNet : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbNet(QGraphicsItem *parent = 0);
		virtual ~CPcbNet();

		virtual QString				name();
		virtual QString				description();

		QStringList&				pinRefs();

		int							padstacks();
		CGPadstack*					padstack(int idx);
		CGPadstack*					padstack(QString ref);
		QList<CGPadstack*>&			padstacksRef();
		CGWire&						wire();

		void						sort();
		void						dumpLength();

		void						setWidth(double w) {mWidth=w;}
		double						width();
		bool						routed();
		CPcbClass*					netClass();
		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	public slots:
		virtual void				clearCache();
	protected:
		CGPadstack*					closest(int n,QPointF pt);
		CGPadstack*					farthest(int n,QPointF pt);
		void						swap(CGPadstack* p1,CGPadstack* p2);

	private:
		QPainterPath				mShape;
		double						mWidth;			/* trace width */
		QStringList					mPinRefs;
		QList<CGPadstack*>			mPadstacks;		/* pin padstacks */
		CGWire*						mWire;
};

#endif // CPCBNET_H

