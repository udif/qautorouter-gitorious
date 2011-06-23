/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBIMAGE_H
#define CPCBIMAGE_H

#include <QObject>
#include <QString>

#include "cspecctraobject.h"

class CPcbOutline;
class CPcbPin;
class CPcbImage : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbImage(QGraphicsItem *parent = 0);
		virtual ~CPcbImage();

		QString							name();

		int								outlines();
		CPcbOutline*					outline(int comp);

		int								pins();
		CPcbPin*						pin(int comp);

	private:
		int								nOutlines;
		QList<CPcbOutline*>				mOutlines;
		int								nPins;
		QList<CPcbPin*>					mPins;

};

#endif // CPCBIMAGE_H

