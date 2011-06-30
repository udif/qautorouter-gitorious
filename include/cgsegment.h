/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGSEGMENT_H
#define CGSEGMENT_H

#include <QObject>
#include <QPainterPath>
#include <QGraphicsItem>

class CPcbLayer;
class CGSegment : public QObject, public QGraphicsItem
{
	Q_OBJECT
	public:

		typedef enum
		{
			Segment=0,
			Padstack,
			Wire,
			Via
		} tSegment;

		CGSegment(QObject *parent = 0);
		virtual ~CGSegment();

		virtual tSegment			segmentType() {return CGSegment::Segment;}
		virtual bool				isA(CGSegment::tSegment t) {return t==CGSegment::Segment;}

		virtual void				setWidth(double w);
		virtual double				width();

		virtual void				setLayer(CPcbLayer* layer);
		virtual CPcbLayer*			layer();

		void						append(CGSegment* segment);
		int							segments();
		CGSegment*					segment(int idx);

		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

	private:
		QList<CGSegment*>			mSegments;
		double						mWidth;
		CPcbLayer*					mLayer;
};

#endif // CGSEGMENT_H
