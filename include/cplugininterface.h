/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>

class CPcb;
class CPluginInterface
{
	public:
		virtual ~CPluginInterface() {}

		typedef enum {
			RouterPlugin=0,
			PostRouterPlugin,
		} tPluginType;

		virtual tPluginType			type()=0;						/* is the a router or post-router */

		virtual QString				title() const = 0;				/* a brief name for the plugin */
		virtual QString				version() const = 0;			/* return a version number string */
		virtual QString				author() const = 0;				/* name of author + email */
		virtual QString				website() const = 0;			/* the author's website */
		virtual QString				description() const = 0;		/* a brief description of the plugin */

		virtual bool				start(CPcb* pcb)=0;				/** initialize, gets' called once prior to exec() being called */
		virtual void				stop()=0;						/** stop processing */
		virtual bool				exec()=0;						/** get's called repeatedly while exec() returns true, return false to stop */
		virtual QString				elapsed()=0;					/** elapsed time in seconds */
};


Q_DECLARE_INTERFACE(CPluginInterface, "8bit.zapto.org.CAutoRouter.CPluginInterface/1.0")

#endif  // INTERFACES_H

