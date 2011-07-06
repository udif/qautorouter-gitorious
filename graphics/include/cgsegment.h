/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CGSEGMENT_H
#define CGSEGMENT_H

#include <QObject>
#include <QPainterPath>
#include <QGraphicsItem>

class CPcb;
class CPcbNet;
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

		CGSegment(CPcbNet* net);
		virtual ~CGSegment();

		virtual tSegment			segmentType() {return CGSegment::Segment;}
		virtual bool				isA(CGSegment::tSegment t) {return t==CGSegment::Segment;}
		virtual bool				isStatic();

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
		int							segmentIndexOf(CGSegment* segment) {return segmentsRef().indexOf(segment);}
		virtual bool				isEmpty() {return segments()==0;}

		virtual QRectF				boundingRect() const;
		virtual QPainterPath		shape() const;
		virtual void				paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

		virtual CPcbNet*			net();
		virtual bool				selected();

		void						setRouted(bool routed) {mRouted=routed;}
		virtual bool				routed() {return isStatic()?true:mRouted;}

	public slots:
		virtual void				clear();

	protected:
		QList<CGSegment*>&			segmentsRef() {return mSegments;}
		CPcbNet*					mNet;

	private:
		QList<CGSegment*>			mSegments;
		double						mWidth;
		CPcbLayer*					mLayer;
		CGSegment*					mParentSegment;
		QPointF						mOrigin;
		bool						mRouted;
};

#endif // CGSEGMENT_H
