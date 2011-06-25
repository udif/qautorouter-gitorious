/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGVIA_H
#define CGVIA_H

#include <QObject>
#include <QPainterPath>
#include <QString>
#include <QGraphicsItem>
#include <QMap>

#include "cgpadstack.h"

class CGVia : public CGPadstack
{
	Q_OBJECT
	public:
		CGVia(QObject *parent = 0);
		virtual ~CGVia();

	private:

};

#endif // CGVIA_H
