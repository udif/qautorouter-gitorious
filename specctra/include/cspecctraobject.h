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
class CPcbRule;
class CSpecctraObject : public QObject, public QGraphicsItem /* QGraphicsPathItem */
{
	Q_OBJECT
	public:
		CSpecctraObject(QGraphicsItem* parent=NULL);
		CSpecctraObject( const CSpecctraObject& other);
		virtual ~CSpecctraObject();

		virtual QString					name();
		virtual QString					description();
		virtual QString					toText(int lvl=0);

		CSpecctraObject&				copy( const CSpecctraObject& other );
		CSpecctraObject&				operator=( const CSpecctraObject & other )		{return copy( other ); }

		virtual void					setParent(CSpecctraObject* p);
		virtual void					setObjectClass(QString& objectClass);
		virtual void					appendProperty(QString& property);
		virtual void					appendProperty(QStringList& property);
		virtual void					appendChild(CSpecctraObject* child);

		CSpecctraObject*				parentObject(QString o="");
		QString&						objectClass();
		QStringList&					properties();
		int								childCount(QString o);
		QList<CSpecctraObject*>&		children();
		CSpecctraObject*				child(QString o,int idx=0);
		CSpecctraObject*				root();
		CPcb*							pcb();

		virtual QRectF					boundingRect() const;
		virtual void					paint(QPainter*,const QStyleOptionGraphicsItem*, QWidget*);
		virtual void					dump(int lvl=0);

		virtual CPcbRule*				rule();

		static QGraphicsScene*			globalScene();

	signals:
		void							fault(QString txt);

	protected:
		QString							mObjectClass;			/** The class of spectra object i.e. pcb, component, payer, etc.. */
		QStringList						mProperties;			/** The properties of the object */
		QList<CSpecctraObject*>			mChildren;				/** The offspring of this object */
		CSpecctraObject*				mParentObject;			/** The parent object */
		static	QGraphicsScene*			mGlobalScene;			/** The graphics scheen */

};

#endif // CSPECCTRAOBJECT_H

