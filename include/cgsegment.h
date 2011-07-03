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

		void						setOrigin(QPointF pt) {mOrigin=pt;}
		virtual QPointF				origin() {return mOrigin;}
		QPointF						parentOrigin();

		virtual void				setLayer(CPcbLayer* layer);
		virtual CPcbLayer*			layer();

		void						setParentSegment(CGSegment* segment) {mParentSegment=segment;}
		CGSegment*					parentSegment() {return mParentSegment;}

		void						append(CGSegment* segment);
		int							segments();
		CGSegment*					segment(int idx);
		virtual bool				isEmpty() {return segments()==0;}

		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

	public slots:
		virtual void				clear();

	private:
		QList<CGSegment*>			mSegments;
		double						mWidth;
		CPcbLayer*					mLayer;
		CGSegment*					mParentSegment;
		QPointF						mOrigin;
};

#endif // CGSEGMENT_H
