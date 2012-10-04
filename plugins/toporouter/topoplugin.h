/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef TOPOPLUGIN_H
#define TOPOPLUGIN_H

#include <stdlib.h>
#include <QContiguousCacheData>
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QRectF>
#include <QStack>
#include <QVector>
#include <cplugininterface.h>

extern "C" {
#include "toporouter.h"
}

class CPcb;
class CPcbNet;
class CGSegment;
class TopoRouter : public QObject, public CPluginInterface
 {
	Q_OBJECT
	Q_INTERFACES(CPluginInterface)

	public:

		virtual tPluginType			type();							/* is the a router or post-router */

		virtual QString				title() const;					/* a brief name for the plugin */
		virtual QString				version() const;				/* return a version number string */
		virtual QStringList			credits() const;	
		virtual QString				website() const;				/* the author's website */
		virtual QString				description() const;			/* a brief description of the plugin */
		virtual QStringList			license() const;	
    
        virtual bool				start(CPcb* pcb, QSettings *settings);				/** initialize, gets' called once prior to exec() being called */
		virtual void				stop();							/** stop processing */
		virtual bool				exec();							/** get's called repeatedly while exec() returns true, return false to stop */
		virtual QString				elapsed();						/** elapsed time of the run in hh:mm:ss format */
	signals:
		void						status(QString txt);			/** emit a status text */
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

		void						selectNet(CPcbNet* net,bool selected);
		void						select();
		void						route();
                void						route(CPcbNet* net,CGSegment*);

		void						initializeBox();				/** initialize the expanding box */
                void						expandBox();					/** expand the bounding box */
                void                                            getPads();                                      /** Assemble PCB's pads */
                void                                            getNets();                                      /** Assemble PCB's nets */
                QStack<CPcbNet*>&                               netStack()	{return mNetStack;}
                PadType*                                        FindPad(QString PadName, int Layer);

	private:
		CPcb*						mPcb;
		QSettings*					mAutoRouterSettings;
		QDateTime					mStartTime;
		tRunState					mState;
		QStack<CPcbNet*>				mNetStack;						/** the current work stack */
		QRectF						mBoundingBox;					/** the expanding bounding box */
		toporouter_t*					TopoRouterHandle;
		QList<PadType*>					UsedPadList;
};

#endif // TOPOROUTER_H
