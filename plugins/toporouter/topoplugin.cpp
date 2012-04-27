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
#include <cpcbpath.h>
#include <iostream>

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
bool TopoRouter::start(CPcb* pcb)
{
	netStack().clear();
	mPcb = pcb;
	setState(Idle);
	mStartTime = QDateTime::currentDateTime();

        TopoRouterHandle = toporouter_new();

        if ( mPcb != NULL && mPcb->network() != NULL && TopoRouterHandle != NULL)
	{
            CPcbStructure *S = mPcb->structure();
            CPcbBoundary *B = S->boundary();

            TopoSetSettings(1, 800);
            //std::cout << "PCB Size : " << mPcb->structure()->boundary()->path()->x() << std::endl;
            //TopoSetPCBSettings(mPcb->structure()->boundary()->boundingRect().x() * GEDA_SCALE, mPcb->structure()->boundingRect().y() * GEDA_SCALE, mPcb->structure()->layers());
            TopoSetPCBSettings(10000 * GEDA_SCALE, -10000 * GEDA_SCALE, mPcb->structure()->layers());

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
  * @brief Assemble the PCB's pads into toporouter structures
  */
void TopoRouter::getPads()
{
    if(pcb())
    {
        QList<CSpecctraObject*> Places = pcb()->collect("place");

        for(int PlaceNum=0; PlaceNum < Places.count(); PlaceNum ++)
        {
            CPcbPlace* Place = (CPcbPlace*)Places.at(PlaceNum);

            /* We've got the absolute position of our component */
            std::cout << "Component: " << qPrintable(Place->unit()) << ", " << Place->pos().x() << " x " << Place->pos().y() << ", rot: " << Place->rotation() << std::endl;

            QTransform Transform;

            /* Apply the global translation (first so will be applied last) */
            Transform.translate(Place->pos().x(), Place->pos().y());

            /* Apply the rotation to be applied first */
            Transform.rotate(Place->rotation(), Qt::ZAxis);

            for(int PadNum = 0; PadNum < Place->pads(); PadNum ++)
            {
                QString PinName = Place->unit();
                qreal OriginX, OriginY;
                CPcbPin* Pin = Place->pin(Place->pad(PadNum)->pinRef());

                std::cout << "Pin " << qPrintable(Place->pad(PadNum)->pinRef()) << ": " << Pin->pos().x() << "," << Pin->pos().y() << std::endl;
                PinName.append("-");
                PinName.append(Place->pad(PadNum)->pinRef());

                for(int ShapeNum = 0; ShapeNum < Pin->padstack()->shapes(); ShapeNum ++)
                {
                    CPcbShape* Shape = Pin->padstack()->shape(ShapeNum);
                    int Layer;
                    qreal X1,Y1,X2,Y2;
                    qreal XOut1, YOut1, XOut2, YOut2;


                    Shape->shape().boundingRect().getCoords(&X1, &Y1, &X2, &Y2);


                    OriginX = Pin->pos().x();
                    OriginY = Pin->pos().y();

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
                    Transform.map(OriginX, OriginY, &OriginX, &OriginY);

                    /* And to be like geda we need to be 0.01 mil */
                    XOut1 *= GEDA_SCALE;
                    YOut1 *= GEDA_SCALE;
                    XOut2 *= GEDA_SCALE;
                    YOut2 *= GEDA_SCALE;
                    OriginX *= GEDA_SCALE;
                    OriginY *= GEDA_SCALE;

                    std::cout << "Shape " << qPrintable(PinName) << " Transformed: " << XOut1 << "," << YOut1 << " x " << XOut2 << "," << YOut2 << std::endl;

                    UsedPadList.append(AddPad(TopoRouterHandle, (char *)qPrintable(PinName),
                            XOut1, YOut1, XOut2, YOut2, XOut2-XOut1, 1500, SQUAREFLAG, Layer,
                            OriginX, OriginY));
                }
            }

        }
        for(int i = 0; i < mPcb->structure()->layers(); i++)
        {
            build_cdt(TopoRouterHandle, &(TopoRouterHandle->layers[i]));
        }
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
    QList<RatLineType *> Rats;

    if(pcb())
    {
        TopoRouterHandle->bboxtree = gts_bb_tree_new(TopoRouterHandle->bboxes);

        QList<CSpecctraObject*> Nets = pcb()->collect("net");

        for(int NetNum=0; NetNum < Nets.count(); NetNum ++)
        {
            CPcbNet* Net = (CPcbNet*)Nets.at(NetNum);
            QString NetName = Net->name();
            NetName.replace("\"", "");
            std::cout << "New net " << qPrintable(NetName) << std::endl;

            // No idea what the last argument (char *Style) is and why we need it.. NULL in examples I've looked at so far
            toporouter_netlist_t *TopoNetList = netlist_create(TopoRouterHandle, (char *)qPrintable(NetName), NULL);

            if(Net->padstacks() > 0)
            {
                PadType *Pad1;

                for(int PadStackNum = 0; PadStackNum < Net->padstacks(); PadStackNum ++)
                {
                    CGPadstack *PadStack = Net->padstack(PadStackNum);

                    PadType *Pad = FindPad(PadStack->unitRef(), 0);

                    // Presuming we only have one segment.. Can we have more??
                    std::cout << "Pin: " << qPrintable(PadStack->unitRef()) << ", Pos: " << Pad->Origin.X << "," << Pad->Origin.Y << std::endl;

                    toporouter_cluster_t *TopoCluster = cluster_create(TopoRouterHandle, TopoNetList);

    #warning Nets should be supporting more than pads
    #warning Last arg (0) is layer.. We should support other layers
                    toporouter_bbox_t *TopoBox = toporouter_bbox_locate(TopoRouterHandle, PAD, Pad,
                        Pad->Origin.X, Pad->Origin.Y, 0);

                    if(TopoBox)
                    {
                        cluster_join_bbox(TopoCluster, TopoBox);
                    }
                    else
                    {
                        std::cout << "Could not locate pad" << std::endl;
                    }

                    /* Create data for our ratsnest */
                    /* Each from line is padstack 0 */
                    if(0 == PadStackNum)
                    {
                        Pad1 = Pad;
                    }
                    else
                    {
                        RatLineType *Rat = (RatLineType *)malloc(sizeof(RatLineType));
                        if(!Rat)
                        {
                            printf("Could not allocate memory for rat\n");
                            exit(1);
                        }
                        Rat->Pad1 = Pad1;
                        Rat->Pad2 = Pad;
                        Rats.append(Rat);
                    }
                }
            }
        }

        /* Now add all our rats */
        RatLineType *Rat;
        while(Rats.count())
        {
            Rat = Rats.takeFirst();

            AddRat(TopoRouterHandle, Rat->Pad1->Origin.X, Rat->Pad1->Origin.Y, Rat->Pad2->Origin.X, Rat->Pad2->Origin.Y, Rat->Pad1->Group, Rat->Pad2->Group);
            free(Rat);
        }
    }
}

/**
  * @brief route the current net.
  */
void TopoRouter::route()
{

    if(pcb())
    {
        getPads();
        getNets();
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

