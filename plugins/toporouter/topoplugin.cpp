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
                qDebug() << "------------------" << __FUNCTION__ << ": ------------------";
                
                QString PinName = Place->unit();
                qreal OriginX, OriginY;
                CPcbPin* Pin = Place->pin(Place->pad(PadNum)->pinRef());

                std::cout << "Pin " << qPrintable(Place->pad(PadNum)->pinRef()) << ": " << Pin->pos().x() << "," << Pin->pos().y() << std::endl;
                PinName.append("-");
                PinName.append(Place->pad(PadNum)->pinRef());

                for(int ShapeNum = 0; ShapeNum < Pin->padstack()->shapes(); ShapeNum ++)
                {
                    CPcbShape* Shape = Pin->padstack()->shape(ShapeNum);
                    unsigned int Layer;
                    qreal X1,Y1,X2,Y2;
                    qreal XOut1, YOut1, XOut2, YOut2, Thickness, Radius;
                    Thickness=mAutoRouterSettings->value("Thickness",1500).toDouble();;
                    Radius=mAutoRouterSettings->value("Radius",100).toDouble();;

                    Shape->shape().boundingRect().getCoords(&X1, &Y1, &X2, &Y2);


                    OriginX = Pin->pos().x();
                    OriginY = Pin->pos().y();
                    qDebug() << "\tShape: " << X1 << "," << Y1 << " x " << X2 << "," << Y2;
                    qDebug() << "\tLayer: " << qPrintable(Shape->layer());

#warning Add proper layer support

                    Layer=(Shape->layer().contains("Front"))?0:(mPcb->structure()->layers()-1);
                    
                    /* For this pad, calculate where it is relative to the component centre */
                    X1 += Pin->pos().x();
                    X2 += Pin->pos().x();
                    Y1 += Pin->pos().y();
                    Y2 += Pin->pos().y();
                    qDebug() << "\t\tShapeTranslated: " << X1 << "," << Y1 << " x " << X2 << "," << Y2;

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

                    qDebug() << "\t\tShape " << qPrintable(PinName) << " Transformed: " << XOut1 << "," << YOut1 << " x " << XOut2 << "," << YOut2;

                    PadType* tmpPad = addPad(TopoRouterHandle, (char *)qPrintable(PinName),
                             XOut1, YOut1, XOut2, YOut2, Radius, Thickness, SQUAREFLAG, Layer );
                    
//                     if(tmpPad!=NULL) UsedPadList.append(tmpPad);
                }
                
                qDebug() << "----------------------------------------------";
            }

        }
        for(int i = 0; i < mPcb->structure()->layers(); i++)
        {
            qDebug() << "build_cdt, run " << i;
            build_cdt(TopoRouterHandle, &(TopoRouterHandle->layers[i]));
        }
    }
}

PadType *TopoRouter::addPad(toporouter_t *r, char *Name,
                              qreal &P1X, qreal &P1Y, qreal &P2X, qreal &P2Y, qreal& Thickness,
                              qreal& Radius, int Shape, unsigned int& Layer)
{
    toporouter_spoint_t p[2], rv[5];
    gdouble x[2], y[2], t, m;
    GList *objectconstraints;
    toporouter_layer_t *l = &(r->layers[Layer]);
    GList *vlist = NULL;
    toporouter_bbox_t *bbox = NULL;
    PadType *NewPad = (PadType *)g_malloc(sizeof(PadType));
#warning Memory leak: This is never freed!
    objectconstraints = NULL;

    if(!NewPad)
    {
        printf("Could not allocate memory for pad\n");
        exit(1);
    }
    t = Thickness / 2.0f;
    x[0] = P1X;
    x[1] = P2X;
    y[0] = P1Y;
    y[1] = P2Y;

#warning Memory leak: String needs to be freed
    NewPad->Name = strdup(Name);
    if(!NewPad->Name)
    {
        printf("Could not allocate memory for string\n");
        exit(1);
    }
    NewPad->Thickness = Thickness;
    NewPad->Point1.X = P1X;
    NewPad->Point1.Y = P1Y;
    NewPad->Point2.X = P2X;
    NewPad->Point2.Y = P2X;

    if(SQUAREFLAG == Shape)
    {
        qDebug() << __FUNCTION__ << " Square or oblong pad. Four points and four constraint edges are used";
        /* Square or oblong pad. Four points and four constraint edges are
        * used */
        if(x[0] == x[1] && y[0] == y[1])
        {
            qDebug() << __FUNCTION__ << " Pad is square";
            /* Pad is square */
            vlist = rect_with_attachments(Radius,
                x[0]-t, y[0]-t,
                x[0]-t, y[0]+t,
                x[0]+t, y[0]+t,
                x[0]+t, y[0]-t,
                Layer);

            bbox = toporouter_bbox_create(Layer, vlist, PAD, NewPad);
            r->bboxes = g_slist_prepend(r->bboxes, bbox);
            insert_constraints_from_list(r, l, vlist, bbox);

            g_list_free(vlist);

            bbox->point = GTS_POINT( insert_vertex(r, l, x[0], y[0], bbox) );
            g_assert(TOPOROUTER_VERTEX(bbox->point)->bbox == bbox);
        }
        else
        {
            qDebug() << __FUNCTION__ << "  Pad is diagonal oblong or othogonal oblong";
            /* Pad is diagonal oblong or othogonal oblong */

            m = cartesian_gradient(x[0], y[0], x[1], y[1]);

            p[0].x = x[0]; p[0].y = y[0];
            p[1].x = x[1]; p[1].y = y[1];

            vertex_outside_segment(&p[0], &p[1], t, &rv[0]);
            vertices_on_line(&rv[0], perpendicular_gradient(m), t, &rv[1], &rv[2]);

            vertex_outside_segment(&p[1], &p[0], t, &rv[0]);
            vertices_on_line(&rv[0], perpendicular_gradient(m), t, &rv[3], &rv[4]);

            if(wind(&rv[1], &rv[2], &rv[3]) != wind(&rv[2], &rv[3], &rv[4]))
            {
                rv[0].x = rv[3].x; rv[0].y = rv[3].y;
                rv[3].x = rv[4].x; rv[3].y = rv[4].y;
                rv[4].x = rv[0].x; rv[4].y = rv[0].y;
            }

            vlist = rect_with_attachments(Radius,
                rv[1].x, rv[1].y,
                rv[2].x, rv[2].y,
                rv[3].x, rv[3].y,
                rv[4].x, rv[4].y,
                Layer);

            qDebug() << __FUNCTION__ << "toporouter_bbox_create(Layer, vlist, PAD, NewPad)";
            bbox = toporouter_bbox_create(Layer, vlist, PAD, NewPad);
            r->bboxes = g_slist_prepend(r->bboxes, bbox);
            insert_constraints_from_list(r, l, vlist, bbox);
            /*
            g_list_free(vlist);

            printf("Pad %s: OBLONG : rect_with_attachments(%2.2f, %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f,  %2.2f)\n",
                            NewPad->Name, (double)Radius,
                            (double)rv[1].x, (double)rv[1].y,
                            (double)rv[2].x, (double)rv[2].y,
                            (double)rv[3].x, (double)rv[3].y,
                            (double)rv[4].x, (double)rv[4].y,
                            (double)(Layer));

            //bbox->point = GTS_POINT( gts_vertex_new(vertex_class, (x[0] + x[1]) / 2., (y[0] + y[1]) / 2., 0.) );
            bbox->point = GTS_POINT( insert_vertex(r, l, (x[0] + x[1]) / 2., (y[0] + y[1]) / 2., bbox) );
            g_assert(TOPOROUTER_VERTEX(bbox->point)->bbox == bbox);
            */
        }
    }
    else
    {
        /* Either round pad or pad with curved edges */

        if(x[0] == x[1] && y[0] == y[1])
        {
            /* One point */

            /* bounding box same as square pad */
            vlist = rect_with_attachments(Radius,
            x[0]-t, y[0]-t,
            x[0]-t, y[0]+t,
            x[0]+t, y[0]+t,
            x[0]+t, y[0]-t,
            l - r->layers);
            bbox = toporouter_bbox_create(l - r->layers, vlist, PAD, NewPad);
            r->bboxes = g_slist_prepend(r->bboxes, bbox);
            g_list_free(vlist);

            //bbox->point = GTS_POINT( insert_vertex(r, l, x[0], y[0], bbox) );
            bbox->point = GTS_POINT( insert_vertex(r, l, x[0], y[0], bbox) );

        }
        else
        {
            /* Two points and one constraint edge */

            /* the rest is just for bounding box */
            m = cartesian_gradient(x[0], y[0], x[1], y[1]);

            p[0].x = x[0]; p[0].y = y[0];
            p[1].x = x[1]; p[1].y = y[1];

            vertex_outside_segment(&p[0], &p[1], t, &rv[0]);
            vertices_on_line(&rv[0], perpendicular_gradient(m), t, &rv[1], &rv[2]);

            vertex_outside_segment(&p[1], &p[0], t, &rv[0]);
            vertices_on_line(&rv[0], perpendicular_gradient(m), t, &rv[3], &rv[4]);

            if(wind(&rv[1], &rv[2], &rv[3]) != wind(&rv[2], &rv[3], &rv[4]))
            {
                rv[0].x = rv[3].x; rv[0].y = rv[3].y;
                rv[3].x = rv[4].x; rv[3].y = rv[4].y;
                rv[4].x = rv[0].x; rv[4].y = rv[0].y;
            }

            vlist = rect_with_attachments(Radius,
            rv[1].x, rv[1].y,
            rv[2].x, rv[2].y,
            rv[3].x, rv[3].y,
            rv[4].x, rv[4].y,
            l - r->layers);
            bbox = toporouter_bbox_create(l - r->layers, vlist, PAD, NewPad);
            r->bboxes = g_slist_prepend(r->bboxes, bbox);
            insert_constraints_from_list(r, l, vlist, bbox);
            g_list_free(vlist);

            //bbox->point = GTS_POINT( gts_vertex_new(vertex_class, (x[0] + x[1]) / 2., (y[0] + y[1]) / 2., 0.) );
            bbox->point = GTS_POINT( insert_vertex(r, l, (x[0] + x[1]) / 2., (y[0] + y[1]) / 2., bbox) );

            //bbox->constraints = g_list_concat(bbox->constraints, insert_constraint_edge(r, l, x[0], y[0], x[1], y[1], bbox));
        }
    }
    return NewPad;
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
    QList<RatType *> Rats;

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

#if 0
            if(Net->padstacks() > 0)
            {
                PadType *Pad1;

                for(int PadStackNum = 0; PadStackNum < Net->padstacks(); PadStackNum ++)
                {
                    CGPadstack *PadStack = Net->padstack(PadStackNum);

                    PadType *Pad = FindPad(PadStack->unitRef(), 0);

                    // Presuming we only have one segment.. Can we have more??

                    toporouter_cluster_t *TopoCluster = cluster_create(TopoRouterHandle, TopoNetList);

    #warning Nets should be supporting more than pads
    #warning Last arg (0) is layer.. We should support other layers
                    toporouter_bbox_t *TopoBox = toporouter_bbox_locate(TopoRouterHandle, PAD, Pad, Pad->Point1.X, Pad->Point1.Y, 0);

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
                        RatType *Rat = (RatType *)malloc(sizeof(RatType));
                        if(!Rat)
                        {
                            printf("Could not allocate memory for rat\n");
                            exit(1);
                        }
                        Rat->Point1 = Pad1->Point1;
                        Rat->Point2 = Pad->Point1;
                        Rats.append(Rat);
                    }
                }
            }
        }
#endif

        /* Now add all our rats */
#if 0
        RatType *Rat;
        while(Rats.count())
        {
            Rat = Rats.takeFirst();

            AddRat(TopoRouterHandle, Rat->Point1.X, Rat->Point2.Y, Rat->Point2.X, Rat->Point2.Y, Rat->group1, Rat->group1);
            free(Rat);
        }
#endif
        }
}}

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

