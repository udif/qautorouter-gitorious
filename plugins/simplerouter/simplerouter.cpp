/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "simplerouter.h"

#include <cspecctraobject.h>
#include <cpcb.h>
#include <cpcbnetwork.h>
#include <cpcbnet.h>
#include <cpcbstructure.h>
#include <cpcbboundary.h>
#include <cutil.h>
#include <cgsegment.h>
#include <cgwire.h>

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
			case Selecting:		msg+="Select";	break;
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
	netStack().clear();
	mPcb = pcb;
	setState(Idle);
	mStartTime = QDateTime::currentDateTime();
	if ( mPcb != NULL && mPcb->network() != NULL )
	{
		setState(Selecting);
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
  * @brief select a net for routing.
  */
void SimpleRouter::select()
{
	netStack().clear();
	if ( pcb() != NULL && pcb()->network() != NULL )
	{
		for(int n=0; n < pcb()->network()->nets(); n++)
		{
			CPcbNet* net = pcb()->network()->net(n);
			if ( !net->routed() )
			{
				netStack().push(net);
				setState(Routing);
			}
			pcb()->yield();
		}
	}
	if (netStack().isEmpty())
		stop();
}

/**
  * @brief route a segment
  */
void SimpleRouter::route(CGSegment* segment)
{
	segment->setWidth(20);
	segment->setRouted(true);
	for(int n=0; n < segment->segments(); n++)
	{
		CGSegment* child = segment->segment(n);
		route(child);
	}
}

/**
  * @brief route the current net.
  */
void SimpleRouter::route()
{
	while( netStack().count() )
	{
		CPcbNet* net = netStack().pop();
		CGWire& wire = net->wire();
		net->setSelected(true);
		net->update();
		for( int n=0; running() && n < wire.segments(); n++)
		{
			CGSegment* segment = wire.segment(n);
			route(segment);
			segment->update();
			pcb()->yield();				/* give up some CPU to the main app */
			emit status(currentStatus());
		}
		net->setSelected(false);
		net->update();
	}
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
			setState(Selecting);
			break;
		case Idle:
			rc=false;
			break;
		case Selecting:
			select();
			if ( running() )
				setState(Routing);
			break;
		case Routing:
			route();
			if ( running() )
				setState(Selecting);
			break;
	}
	return rc;
}

Q_EXPORT_PLUGIN2(simplerouter, SimpleRouter);

