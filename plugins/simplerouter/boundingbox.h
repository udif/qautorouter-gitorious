/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QObject>
#include <QSizeF>
#include <QPointF>

#include <cpcb.h>
#include <cpcbnet.h>
#include <cgsegment.h>

class BoundingBox : public QObject
{
		Q_OBJECT

	public:
		BoundingBox(CPcb* pcb, CPcbNet* net, CGSegment* segment, QObject* parent=NULL);
		BoundingBox(const BoundingBox& other);
		virtual ~BoundingBox();

		BoundingBox&				copy(const BoundingBox& other);


		CPcb*						pcb()				{return mPcb;}
		CPcbNet*					net()				{return mNet;}
		CGSegment*					segment()			{return mSegment;}

		QRectF&						rect()				{return mRect;}

		void						route();

	public slots:
		void						setPcb(CPcb* pcb)				{mPcb=pcb;}
		void						setNet(CPcbNet* net)			{mNet=net;}
		void						setSegment(CGSegment* segment)	{mSegment=segment;}

	protected slots:
		QRectF						grow();
		void						route(CPcbNet* net,CGSegment*);

	private:
		QRectF						mRect;
		CPcb*						mPcb;
		CPcbNet*					mNet;
		CGSegment*					mSegment;

};

#endif // BOUNDINGBOX_H
