/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "topoplugin.h"
#include "../../../qautorouter/specctra/include/cpcbpin.h"

#include <cspecctraobject.h>
#include <cpcb.h>
#include <cpcbnetwork.h>
#include <cpcbnet.h>
#include <cpcbpin.h>
#include <cpcbstructure.h>
#include <cpcbboundary.h>
#include <cgpadstack.h>
#include <cutil.h>
#include <cgsegment.h>
#include <cgwire.h>
#include <cpcbpadstack.h>
#include <cpcbshape.h>
#include <cpcbplace.h>
#include <cpcbplacement.h>
#include <cpcbpath.h>

#include <iostream>
#include <QDebug>

#define GEDA_SCALE 100

/**
  * @return plugin type
  */
CPluginInterface::tPluginType TopoRouter::type()
{
	return CPluginInterface::RouterPlugin;
}

/**
  * @return A title for the plugin to display in user interface.
  */
QString TopoRouter::title() const
{
	return "Topological Router";
}

/**
  * @return A version for the plugin to display in user interface.
  */
QString TopoRouter::version() const
{
	return "1.0";
}

/**
  * @return A author for the plugin to display in user interface.
  */
QStringList TopoRouter::credits() const
{
	QStringList rc;
	rc << "Anthony Blake <tonyb33@gmail.com>";
	rc << "Peter Allen <pete@strangepete.co.uk>";
	rc << "Mike Sharkey <michael_sharkey@firstclass.com>";
	return rc;
}

/**
  * @return A website for the plugin to display in user interface.
  */
QString TopoRouter::website() const
{
	return "http://qautorouter.sourceforge.net";
}

/**
  * @return A short description of the plugin to display in user interface.
  */
QString TopoRouter::description() const
{
	return "Topological Auto Router";
}

/**
  * @brief The license text
  */
QStringList TopoRouter::license() const
{
	QStringList rc;
	rc << "GPL Version 2.0";
	rc << "This program is free software; you can redistribute it and/or modify\n"
			"it under the terms of the GNU General Public License as published\n"
			"by the Free Software Foundation; either version 2 of the License,\n"
			"or (at your option) any later version. \n"
			"\n"
			"This program is distributed in the hope that it will be useful, \n"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the \n"
			"GNU General Public License for more details. \n"
			"\n"
			"You should have received a copy of the GNU General Public License \n"
			"along with this program; if not, write to the \n"
			"\n"
			"Free Software Foundation, Inc.,\n"
			"59 Temple Place - Suite 330,\nBoston, MA 02111-1307,\nU.S.A.\n";
	return rc;
}

/**
  * @return elapsed time to string
  */
QString TopoRouter::elapsed()
{
	return CUtil::elapsed(mStartTime.secsTo(QDateTime::currentDateTime()));
}

/**
  * @return current state
  */
TopoRouter::tRunState TopoRouter::state()
{
	return mState;
}

/**
  * @brief set surrent run state
  */
void TopoRouter::setState(tRunState state)
{
	mState=state;
}

/**
  * @return A status message for the status line
  */
QString TopoRouter::currentStatus()
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
bool TopoRouter::start(CPcb* pcb, QSettings* settings )
{
	netStack().clear();
	mPcb = pcb;
	setState(Idle);
	mStartTime = QDateTime::currentDateTime();
        mAutoRouterSettings=settings;

        TopoRouterHandle = toporouter_new();
        TopoRouterHandle->viacost=mAutoRouterSettings->value("ViaCosts").toDouble();

        if ( mPcb != NULL && mPcb->network() != NULL && TopoRouterHandle != NULL)
        {
            CPcbStructure *S = mPcb->structure();
            CPcbBoundary *B = S->boundary();

//             TopoSetSettings(settings->value("KeepAway").toDouble(), settings->value("LineThickness").toDouble());
            //std::cout << "PCB Size : " << mPcb->structure()->boundary()->path()->x() << std::endl;
            //TopoSetPCBSettings(mPcb->structure()->boundary()->boundingRect().x() * GEDA_SCALE, mPcb->structure()->boundingRect().y() * GEDA_SCALE, mPcb->structure()->layers());
//             TopoSetPCBSettings(10000 * GEDA_SCALE, -10000 * GEDA_SCALE, mPcb->structure()->layers());
//             AllocateLayers(TopoRouterHandle, mPcb->structure()->layers());
            hybrid_router(TopoRouterHandle);

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
void TopoRouter::stop()
{
	setState(Idle);
	QObject::disconnect(this,SIGNAL(status(QString)),mPcb,SIGNAL(status(QString)));
	QObject::disconnect(this,SIGNAL(clearCache()),mPcb,SLOT(clearCache()));
}

/**
  * @brief select a net
  */
void TopoRouter::selectNet(CPcbNet *net, bool selected)
{
	net->setSelected(selected);
	net->update();
	pcb()->yield();				/* give up some CPU to the main app */
}

/**
  * @brief select a net for routing.
  */
void TopoRouter::select()
{
	netStack().clear();
	if ( pcb() != NULL && pcb()->network() != NULL )
	{
		for(int n=0; running() && n < pcb()->network()->nets(); n++)
		{
			CPcbNet* net = pcb()->network()->net(n);
			if ( !net->routed() )
			{
				netStack().push(net);
				setState(Routing);
			}
		}
	}
	if (netStack().isEmpty())
		stop();
}

/**
  * @brief route a segment
  */

void TopoRouter::route(CPcbNet* net, CGSegment* segment)
{
}

/**
  * @brief Assemble the PCB's pads into toporouter structures
  */
void TopoRouter::getPads()
{
	 if(pcb()) {
	 	qDebug() << "Feeding our SPECTRA objects to the gEDA-format PCB";
		if(PCB==NULL)
			PCB=new PCBType();

		if(PCB->Data==NULL)
			PCB->Data=new DataType();

		QList<CSpecctraObject*> Places = pcb()->collect("place");
		for(int PlaceNum=0; PlaceNum < Places.count(); PlaceNum ++) {
			//Iterates over the different part case type groups (SOIC,DIP,etc.)
			ElementType* element = new ElementType();
			CPcbPlace* Place = (CPcbPlace*)Places.at(PlaceNum);
			for(int PadNum = 0; PadNum < Place->pads(); PadNum ++) {
				CPcbPin* Pin = Place->pin(Place->pad(PadNum)->pinRef());
				PadType* pad = new PadType();
				pad->Point1.X=Pin->x();
				pad->Point2.X=Pin->x();
				pad->Point1.Y=Pin->y();
				pad->Point2.Y=Pin->y();
				g_list_append(element->Pad,pad);
				for(int ShapeNum = 0; ShapeNum < Pin->padstack()->shapes(); ShapeNum ++) {
					qDebug() << "Pad stack loop";
				}
			}
			g_list_append(PCB->Data->Element,element);
		}
		import_geometry(TopoRouterHandle);
	}
}

/**
  * @brief Find a pad from PadName like "R2-2" on layer Layer.. Found in QList UsedPads
  */
PadType *TopoRouter::FindPad(QString PadName, int Layer)
{
    int i = 0;
    PadType *Ret = NULL;

    while((i < UsedPadList.count()) && (!Ret))
    {
        PadType *Pad = (PadType *)UsedPadList.at(i);
        if(0 == PadName.compare(Pad->Name))
        {
            //std::cout << "Pad compare found: [" << Pad->Name << "][" << qPrintable(PadName) << "]" << std::endl;
            Ret = Pad;
        }
        i ++;
    }
    if(NULL == Ret)
    {
        std::cout << "Couldn't find pad " << qPrintable(PadName) << std::endl;
    }
    return Ret;
}

/**
  * @brief Assemble the PCB's nets into toporouter structures
  */
void TopoRouter::getNets()
{
}

/**
  * @brief route the current net.
  */
void TopoRouter::route()
{
    if(pcb())
    {
        qDebug() << "Starging routing the net";
        getPads();
        getNets();
        qDebug() << "\tNumber of nets: " << TopoRouterHandle->netlists->len;
//         toporoute(TopoRouterHandle);
        hybrid_router(TopoRouterHandle);
        setState(Idle);
    }
}

/**
  * @brief run for a little bit.
  */
bool TopoRouter::exec()
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

Q_EXPORT_PLUGIN2(TopoRouter, TopoRouter);

