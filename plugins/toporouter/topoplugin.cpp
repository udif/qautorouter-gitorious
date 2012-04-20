/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "topoplugin.h"

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
#include <iostream>

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
bool TopoRouter::start(CPcb* pcb)
{
	netStack().clear();
	mPcb = pcb;
	setState(Idle);
	mStartTime = QDateTime::currentDateTime();

        TopoRouterHandle = toporouter_new();

        if ( mPcb != NULL && mPcb->network() != NULL && TopoRouterHandle != NULL)
	{
            AllocateLayers(TopoRouterHandle, mPcb->structure()->layers());
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
  #if 0
    {
            CPcbNet* net = netStack().pop();
            CGWire& wire = net->wire();
            selectNet(net,true);

            for( int n=0; running() && n < wire.segments(); n++)
            {
                    CGSegment* segment = wire.segment(n);
                    route(net,segment);
                    emit status(currentStatus());
            }
            selectNet(net,false);
    }
    segment->setWidth(20);
        segment->setRouted(true);
        segment->update();
	pcb()->yield();				/* give up some CPU to the main app */
        for(int n=0; running() && n < segment->segments(); n++)
        {
                CGSegment* child = segment->segment(n);
                route(net,child);
        }
#endif
}

/**
  * @brief route the current net.
  */
void TopoRouter::route()
{
    if(pcb())
    {
        QList<CSpecctraObject*> Places = pcb()->collect("place");

        for(int PlaceNum=0; PlaceNum < Places.count(); PlaceNum ++)
        {
            CPcbPlace* Place = (CPcbPlace*)Places.at(PlaceNum);
            
            /* We've got the absolute position of our component */
            std::cout << "Component: " << Place->pos().x() << " x " << Place->pos().y() << ", rot: " << Place->rotation() << std::endl;
            
            QTransform Transform;

            /* Apply the global translation (first so will be applied last) */
            Transform.translate(Place->pos().x(), -Place->pos().y());

            /* Apply the rotation to be applied first */
            Transform.rotate(Place->rotation(), Qt::ZAxis);

            for(int PadNum = 0; PadNum < Place->pads(); PadNum ++)
            {
                CPcbPin* Pin = Place->pin(Place->pad(PadNum)->pinRef());
                std::cout << "Pin " << qPrintable(Place->pad(PadNum)->pinRef()) << ": " << Pin->pos().x() << "," << Pin->pos().y() << std::endl;

                for(int ShapeNum = 0; ShapeNum < Pin->padstack()->shapes(); ShapeNum ++)
                {
                    CPcbShape* Shape = Pin->padstack()->shape(ShapeNum);
                    int Layer;
                    qreal X1,Y1,X2,Y2;
                    qreal XOut1, YOut1, XOut2, YOut2;

                    Shape->shape().boundingRect().getCoords(&X1, &Y1, &X2, &Y2);

                    std::cout << "Shape: " << X1 << "," << Y1 << " x " << X2 << "," << Y2 << std::endl;
                    std::cout << "    Layer: " << qPrintable(Shape->layer()) << std::endl;

#warning Add proper layer support
                    if(Shape->layer().contains("Front"))
                    {
                        Layer = 0;
                    }
                    else
                    {
                        Layer = 1;
                    }

                    /* For this pad, calculate where it is relative to the component centre */
                    X1 += Pin->pos().x();
                    X2 += Pin->pos().x();
                    Y1 += Pin->pos().y();
                    Y2 += Pin->pos().y();
                    std::cout << "ShapeTranslated: " << X1 << "," << Y1 << " x " << X2 << "," << Y2 << std::endl;

                    /* Then transform it by the component position / orientation matrix */
                    Transform.map(X1, Y1, &XOut1, &YOut1);
                    Transform.map(X2, Y2, &XOut2, &YOut2);

                    /* And to be like geda we need to be 0.01 mil */
                    XOut1 *= 100;
                    YOut1 *= 100;
                    XOut2 *= 100;
                    YOut2 *= 100;

                    std::cout << "ShapeTransformed: " << XOut1 << "," << YOut1 << " x " << XOut2 << "," << YOut2 << std::endl;

                    AddPad(TopoRouterHandle, (char *)qPrintable(Place->pad(PadNum)->pinRef()),
                            XOut1, YOut1, XOut2, YOut2, XOut2-XOut1, 1500, SQUAREFLAG, Layer);
                }
            }

        }
        toporoute(TopoRouterHandle);
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

