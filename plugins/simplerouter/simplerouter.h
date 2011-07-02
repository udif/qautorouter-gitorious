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

#include <cplugininterface.h>

class CPcb;
class CPcbNet;
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
		virtual QString				status();						/** a brief status report for the status bar */
		virtual QString				elapsed();						/** elapsed time of the run in hh:mm:ss format */

	protected:
		typedef enum {
			Idle,													/** there is nothing happening */
			SortingNets,											/** sorting netlists */
			Selecting,												/** selecting which net(s) to route */
			Searching,												/** searching for a route on curent net(s) */
			Routing,												/** committing a route */
		} tRunState;
		CPcb*						pcb() {return mPcb;}
		tRunState					state() {return mState;}
		void						setState(tRunState state) {mState=state;}
		QList<CPcbNet*>				nets() {return mNets;}
		CPcbNet*					net(int idx) {return mNets.at(idx);}

		void						sort();
		void						select();
		void						search();
		void						route();

	private:
		CPcb*						mPcb;
		QDateTime					mStartTime;
		tRunState					mState;
		QList<CPcbNet*>				mNets;

};

#endif // SIMPLEROUTER_H
