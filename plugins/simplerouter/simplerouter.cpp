/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "simplerouter.h"

#include <QString>

#include <cspecctraobject.h>
#include <cpcb.h>
#include <cpcbnetwork.h>

/**
  * @return plugin type
  */
CPluginInterface::tPluginType SimpleRouter::type()
{
	return CPluginInterface::RouterPlugin;
}

/**
  * @return A title for the plugin to display in user interface.
  */
QString SimpleRouter::title() const
{
	return "Simple Router";
}

/**
  * @return A version for the plugin to display in user interface.
  */
QString SimpleRouter::version() const
{
	return "1.0";
}

/**
  * @return A author for the plugin to display in user interface.
  */
QString SimpleRouter::author() const
{
	return "Mike Sharkey <mike@pikeaero.com>";
}

/**
  * @return A website for the plugin to display in user interface.
  */
QString SimpleRouter::website() const
{
	return "http://qautorouter.sourceforge.net";
}

/**
  * @return A short description of the plugin to display in user interface.
  */
QString SimpleRouter::description() const
{
	return "A Simple Auto Router Skeleton";
}

/**
  * @return elapsed time to string
  */
QString SimpleRouter::elapsed()
{
	QString rc;
	QDateTime now = QDateTime::currentDateTime();
	int tm,hour,min,sec;
	tm = mStartTime.secsTo(now);
	hour=tm/3600;
	tm=tm%3600;
	min=tm/60;
	tm=tm%60;
	sec=tm;
	rc.sprintf("%02d:%02d:%02d",hour,min,sec);
	return rc;
}

/**
  * @return A status message for the status line
  */
QString SimpleRouter::status()
{
	QString msg;
	if ( pcb() != NULL && pcb()->network() != NULL )
	{
		msg += elapsed()+tr(" Nets: ")+QString::number(pcb()->network()->nets()) + " " + tr("Routed: ")+QString::number(pcb()->network()->routed());
	}
	return msg;
}

/**
  * @brief perform initialization
  */
bool SimpleRouter::initialize(CPcb* pcb)
{
	mPcb = pcb;
	mStartTime = QDateTime::currentDateTime();
	return( mPcb != NULL );
}

/**
  * @brief run for a little bit.
  */
bool SimpleRouter::exec()
{
	bool rc=true;
	return rc;
}



Q_EXPORT_PLUGIN2(simplerouter, SimpleRouter);

