/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBRENDER_H
#define CPCBRENDER_H

#include "cspecctraobject.h"
#include "cpcb.h"
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QString>

class CPcbRender : public QObject
{
	Q_OBJECT
	public:
		explicit CPcbRender(CPcb* pcb, QGraphicsView* view, QObject *parent = 0);

		QGraphicsView*				view() {return mPcbView;}
		QGraphicsScene*				scene() {return view()->scene();}

		void						renderOutline();
		void						renderOutlines();
		void						renderPins();

	signals:
		void						fault(QString txt);

	private:
		CPcb*						mPcb;
		QGraphicsView*				mPcbView;

};

#endif // CPCBRENDER_H
