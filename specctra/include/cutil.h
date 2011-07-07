/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CUTIL_H
#define CUTIL_H

#include <QString>
#include <QList>
#include <QPointF>

class CGPadstack;
class CPcbNet;
class CUtil
{
	public:
		typedef enum {
			Ascending,
			Descending,
		} tSortOrder;
		static QString				elapsed(int tm);
		static QString				unitRef(QString unitPinRef);
		static QString				pinRef(QString unitPinRef);
		static QString				placeRef(QString unitRef,QString pinRef);
		static int					sort(QList<CGPadstack*>& pads,QPointF pt,tSortOrder order=Ascending);
		static int					sort(QList<CPcbNet*>& nets,QPointF pt,tSortOrder order=Ascending);
};

#endif // CUTIL_H
