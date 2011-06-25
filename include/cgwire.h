/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGWIRE_H
#define CGWIRE_H

#include <QObject>
#include <QList>

class CGSegment;
class CGWire : public QObject
{
	Q_OBJECT
	public:
		CGWire(QObject *parent = 0);
		virtual ~CGWire();

		int								segments();
		CGSegment*						segment(int idx);

	private:
		QList<CGSegment*>				mSegments;
};

#endif // CGWIRE_H
