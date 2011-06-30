/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGWIRE_H
#define CGWIRE_H

#include <QObject>
#include <QList>

#include "cgsegment.h"

class CGWire : public CGSegment
{
	Q_OBJECT
	public:
		CGWire(QObject *parent = 0);
		virtual ~CGWire();

		virtual tSegment			segmentType() {return CGSegment::Wire;}
		virtual bool				isA(CGSegment::tSegment t) {return t==CGSegment::Wire || CGSegment::isA(t);}

		virtual QPainterPath		shape() const;
};

#endif // CGWIRE_H
