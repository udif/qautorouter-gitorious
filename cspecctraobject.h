/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CSPECCTRAOBJECT_H
#define CSPECCTRAOBJECT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QPaintEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <math.h>
#define pi() (3.1415)
#define radian(d) (d*(pi()/180))

class CPcb;
class CSpecctraObject : public QObject, public QGraphicsItem /* QGraphicsPathItem */
{
	Q_OBJECT
	public:
		CSpecctraObject(QGraphicsItem* parent=NULL);
		CSpecctraObject( const CSpecctraObject& other,QGraphicsItem* paernt=NULL);
		virtual ~CSpecctraObject();

		virtual QString					toText(int lvl=0);

		CSpecctraObject&				copy( const CSpecctraObject& other );
		CSpecctraObject&				operator=( const CSpecctraObject & other )		{return copy( other ); }

		virtual void					setParent(CSpecctraObject* p)			{mParentObject=p;}
		virtual void					setObjectClass(QString objectClass)		{mObjectClass = objectClass;}
		virtual void					appendProperty(QString property)		{mProperties.append(property);}
		virtual void					appendProperty(QStringList property)	{mProperties += property;}
		virtual void					appendChild(CSpecctraObject* child)		{mChildren.append(child);child->setParent(this);}

		CSpecctraObject*				parentObject()	{return mParentObject;}
		QString							objectClass()	{return mObjectClass;}
		QStringList						properties()	{return mProperties;}
		QList<CSpecctraObject*>			children()		{return mChildren;}
		CSpecctraObject*				child(QString o);
		CSpecctraObject*				root();
		CPcb*							pcb();

		virtual QRectF					boundingRect() const {QRectF r; return r;}
		virtual void					paint(QPainter*,const QStyleOptionGraphicsItem*, QWidget*) {}
		virtual void					dump(int lvl=0);

		static QGraphicsScene*			scene();


	signals:
		void							fault(QString txt);

	protected:
		QString							mObjectClass;			/** The class of spectra object i.e. pcb, component, payer, etc.. */
		QStringList						mProperties;			/** The properties of the object */
		QList<CSpecctraObject*>			mChildren;				/** The offspring of this object */
		CSpecctraObject*				mParentObject;			/** The parent object */
		static	QGraphicsScene*			mScene;					/** The graphics scheen */

};

#endif // CSPECCTRAOBJECT_H

