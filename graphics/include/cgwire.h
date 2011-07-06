/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGWIRE_H
#define CGWIRE_H

#include <QObject>
#include <QList>

#include "cgsegment.h"

class CPcbNet;
class CGWire : public CGSegment
{
	Q_OBJECT
	public:
		CGWire(CPcbNet *net);
		virtual ~CGWire();

		virtual CPcbNet*			net();

		virtual tSegment			segmentType() {return CGSegment::Wire;}
		virtual bool				isA(CGSegment::tSegment t) {return t==CGSegment::Wire || CGSegment::isA(t);}

		virtual bool				insertBreak(QPointF pt,CGSegment::tSegment style);

		virtual QPainterPath		shape() const;
};

#endif // CGWIRE_H
