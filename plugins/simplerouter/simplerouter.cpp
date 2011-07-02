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
  * @return current state
  */
SimpleRouter::tRunState SimpleRouter::state()
{
	return mState;
}

/**
  * @brief set surrent run state
  */
void SimpleRouter::setState(tRunState state)
{
	mState=state;
	emit status(currentStatus());
}

/**
  * @return A status message for the status line
  */
QString SimpleRouter::currentStatus()
{
	QString msg="[";
	if ( pcb() != NULL && pcb()->network() != NULL )
	{
		switch(state())
		{
			default:
			case Idle:			msg+="Idle  ";	break;
			case SortingNets:	msg+="Sort  ";	break;
			case Selecting:		msg+="Select";	break;
			case Searching:		msg+="Search";	break;
			case Routing:		msg+="Route ";	break;
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
	mPcb = pcb;
	setState(Idle);
	mStartTime = QDateTime::currentDateTime();
	if ( mPcb != NULL && mPcb->network() != NULL )
	{
		setState(SortingNets);
		QObject::connect(this,SIGNAL(status(QString)),mPcb,SIGNAL(status(QString)));
		QObject::connect(this,SIGNAL(clearCache()),mPcb,SLOT(clearCache()));
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
	QObject::disconnect(this,SIGNAL(status(QString)),mPcb,SIGNAL(status(QString)));
	QObject::disconnect(this,SIGNAL(clearCache()),mPcb,SLOT(clearCache()));
}

/**
  * Sort padstack lists and net lists.
  */
void SimpleRouter::sort()
{
	if ( pcb() != NULL && pcb()->network() != NULL )
	{
		emit clearCache();
		/* sort the padstack lists associated with each net relative to distance from center point of PCB */
		QPointF centerPt = pcb()->boundingRect().center();
		for( int n=0; n < pcb()->network()->nets(); n++)
		{
			pcb()->network()->net(n)->sort(centerPt);
		}
		/* sort the bet lists based on closeness to PCB center point */
		CUtil::sort(pcb()->network()->netsRef(),centerPt);
		/* redraw */
		CSpecctraObject::scene()->update();
	}
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
	emit status(currentStatus());
	switch(state())
	{
		default:
			setState(SortingNets);
			break;
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

