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

class CPcbNet;
class CGVia : public CGPadstack
{
	Q_OBJECT
	public:
		CGVia(CPcbNet* net);
		virtual ~CGVia();

		virtual tSegment			segmentType() {return CGSegment::Via;}
		virtual bool				isA(CGSegment::tSegment t) {return t==CGSegment::Via || CGPadstack::isA(t);}

	private:

};

#endif // CGVIA_H
