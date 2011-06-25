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

		virtual tPluginType type()=0;

		virtual QString title() const = 0;
		virtual QString version() const = 0;
		virtual QString author() const = 0;
		virtual QString website() const = 0;
		virtual QString description() const = 0;

		virtual bool initialize(CPcb* pcb)=0;		/** initialize, gets' called once prior to exec() being called */
		virtual bool exec()=0;						/** get's called repeatedly while exec() returns true, return false to stop */
};


Q_DECLARE_INTERFACE(CPluginInterface, "8bit.zapto.org.CAutoRouter.CPluginInterface/1.0")

#endif  // INTERFACES_H

