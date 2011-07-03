/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef SIMPLEROUTER_H
#define SIMPLEROUTER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QRectF>
#include <QStack>
#include <cplugininterface.h>

class CPcb;
class CPcbNet;
class CGSegment;
class SimpleRouter : public QObject, public CPluginInterface
 {
	Q_OBJECT
	Q_INTERFACES(CPluginInterface)

	public:

		virtual tPluginType			type();							/* is the a router or post-router */

		virtual QString				title() const;					/* a brief name for the plugin */
		virtual QString				version() const;				/* return a version number string */
		virtual QString				author() const;					/* name of author + email */
		virtual QString				website() const;				/* the author's website */
		virtual QString				description() const;			/* a brief description of the plugin */

		virtual bool				start(CPcb* pcb);				/** initialize, gets' called once prior to exec() being called */
		virtual void				stop();							/** stop processing */
		virtual bool				exec();							/** get's called repeatedly while exec() returns true, return false to stop */
		virtual QString				elapsed();						/** elapsed time of the run in hh:mm:ss format */
	signals:
		void						status(QString txt);			/** emit a status text */
		void						clearCache();					/** clear the object caches */
	protected:
		typedef enum {
			Idle,													/** there is nothing happening */
			Selecting,												/** selecting which net(s) to route */
			Routing,												/** committing a route */
		} tRunState;
		CPcb*						pcb() {return mPcb;}
		tRunState					state();
		bool						running() {return state() != Idle;}
		void						setState(tRunState state);
		QString						currentStatus();				/** a brief status report for the status bar */

		void						select();
		void						route();
		void						route(CGSegment*);

		void						initializeBox();				/** initialize the expanding box */
		void						expandBox();					/** expand the bounding box */

		QStack<CPcbNet*>&			netStack()	{return mNetStack;}

	private:
		CPcb*						mPcb;
		QDateTime					mStartTime;
		tRunState					mState;
		QStack<CPcbNet*>			mNetStack;						/** the current work stack */
		QRectF						mBoundingBox;					/** the expanding bounding box */
};

#endif // SIMPLEROUTER_H
