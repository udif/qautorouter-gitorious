/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef SIMPLEROUTER_H
#define SIMPLEROUTER_H

#include <QObject>
#include <QString>

#include <cplugininterface.h>

class CPcb;
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

		virtual bool				initialize(CPcb* pcb);			/** initialize, gets' called once prior to exec() being called */
		virtual bool				exec();							/** get's called repeatedly while exec() returns true, return false to stop */
		virtual QString				status();						/** a brief status report for the status bar */

	protected:
		CPcb*						pcb() {return mPcb;}

	private:
		CPcb*						mPcb;

};

#endif // SIMPLEROUTER_H
