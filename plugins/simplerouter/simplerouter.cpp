/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "simplerouter.h"

#include <cspecctraobject.h>
#include <cpcb.h>
#include <cpcbnetwork.h>
#include <cpcbnet.h>

#include <cutil.h>

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
	return CUtil::elapsed(mStartTime.secsTo(QDateTime::currentDateTime()));
}

/**
  * @return A status message for the status line
  */
QString SimpleRouter::status()
{
	QString msg="[";
	if ( pcb() != NULL && pcb()->network() != NULL )
	{
		switch(state())
		{
			default:
			case Idle:			msg+="Idle";			break;
			case SortingNets:	msg+="Sorting Nets";	break;
			case Selecting:		msg+="Selecting";		break;
			case Searching:		msg+="Searching";		break;
			case Routing:		msg+="Routing";			break;
		}
		msg += QString("] ")+elapsed()+tr(" Nets: ")+QString::number(pcb()->network()->nets()) + " " + tr("Routed: ")+QString::number(pcb()->network()->routed());
	}
	return msg;
}

/**
  * @brief perform initialization
  */
bool SimpleRouter::start(CPcb* pcb)
{
	mNets.clear();
	mPcb = pcb;
	mState=Idle;
	mStartTime = QDateTime::currentDateTime();
	if ( mPcb != NULL && mPcb->network() != NULL )
	{
		for(int n=0; n < mPcb->network()->nets(); n++)
		{
			mNets.append(mPcb->network()->net(n));
		}
		mState=SortingNets;
		return true;
	}
	return false;
}

/**
  * @brief stop processing
  */
void SimpleRouter::stop()
{
	setState(Idle);
	mNets.clear();
}

void SimpleRouter::sort()
{

}

void SimpleRouter::select()
{
}

void SimpleRouter::search()
{
}

void SimpleRouter::route()
{
}

/**
  * @brief run for a little bit.
  */
bool SimpleRouter::exec()
{
	bool rc=true;
	switch(state())
	{
		default:
		case Idle:
			rc=false;
			break;
		case SortingNets:
			sort();
			setState(Selecting);
			break;
		case Selecting:
			select();
			setState(Searching);
			break;
		case Searching:
			search();
			setState(Routing);
			break;
		case Routing:
			route();
			setState(SortingNets);
			break;
	}
	return rc;
}



Q_EXPORT_PLUGIN2(simplerouter, SimpleRouter);

