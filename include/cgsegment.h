/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGSEGMENT_H
#define CGSEGMENT_H

#include <QObject>
#include <QPainterPath>

class CGSegment : public QObject
{
	Q_OBJECT
	public:
		CGSegment(QObject *parent = 0);
		virtual ~CGSegment();

		virtual QPainterPath			shape();
};

#endif // CGSEGMENT_H
