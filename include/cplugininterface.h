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

		virtual QString title() const = 0;
		virtual QString version() const = 0;
		virtual QString author() const = 0;
		virtual QString website() const = 0;
		virtual QString description() const = 0;

};


Q_DECLARE_INTERFACE(CPluginInterface, "8bit.zapto.org.CAutoRouter.CPluginInterface/1.0")

#endif  // INTERFACES_H

