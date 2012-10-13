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

	PCB=(PCBType*)g_malloc(sizeof(PCBType));
	PCB->Data=(DataType*)g_malloc(sizeof(DataType));
	PCB->Data->LayerN=mPcb->structure()->layers();
// 	PCB->Data->Element=(GList*)g_malloc(sizeof(GList));

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

}

/**
  * @brief Find a pad from PadName like "R2-2" on layer Layer.. Found in QList UsedPads
  */
PadType *TopoRouter::FindPad(QString PadName, int Layer)
{
	PadName=PadName.trimmed();
	int i = 0;
	PadType *Ret = NULL;

	ALLPAD_LOOP(PCB->Data);
	{
		if(PadName.toLocal8Bit().data()==pad->Name)
			Ret=pad;
	}
	END_LOOP;
	END_LOOP;

	if(NULL == Ret)
		qDebug() << "Couldn't find pad " << qPrintable(PadName);

	return Ret;
}

int TopoRouter::FindLayer(QString layer) {
	
}

/**
  * @brief Assemble the PCB's nets into toporouter structures
  */
void TopoRouter::getNets()
{
	if(pcb()) {
	 	qDebug() << "Feeding our SPECTRA nets to the gEDA-format PCB";
		TopoRouterHandle->bboxtree = gts_bb_tree_new(TopoRouterHandle->bboxes);
		QList<CSpecctraObject*> Nets = pcb()->collect("net");
		for(int NetNum=0; NetNum < Nets.count(); NetNum ++)
		{
			qDebug() << "Here the ratnets should be shuffled into the PCB->Data->Rat list";
			CPcbNet* Net = (CPcbNet*)Nets.at(NetNum);
			RatType *line = new RatType();
			line->Point1.X=Net->x();
			line->Point2.X=Net->x();
			line->Point1.Y=Net->y();
			line->Point2.Y=Net->y();
			g_list_append(PCB->Data->Rat,line);
		}
	}
}

void TopoRouter::ConvertRats()
{
}

PadType* TopoRouter::CreatePad(QString PadName,qreal x1, qreal y1, qreal x2, qreal y2)
{
	PadType* newPad = NULL;
	newPad = (PadType*)g_malloc(sizeof(PadType));
	if(newPad==NULL) {
		qDebug() << "Couldn't create pad";
		return NULL;
	}

	newPad->Name=PadName.toLocal8Bit().data();
	newPad->Point1.X=x1;
	newPad->Point2.X=x2;
	newPad->Point1.Y=y1;
	newPad->Point2.Y=y2;

	return newPad;
}

PinType* TopoRouter::CreatePin(QString PinName,QString PinNumber, qreal x, qreal y, unsigned long flag) {
	PinType *newPin = NULL;
	newPin = (PinType*)g_malloc(sizeof(PinType));
	if(newPin==NULL) {
		qDebug() << "Couldn't create pin";
		return NULL;
	}
	newPin->Name=PinName.toLocal8Bit().data();
	newPin->Number=PinNumber.toLocal8Bit().data();
	newPin->X=x;
	newPin->Y=y;
	newPin->Flags.f=flag;

	return newPin;
}

void TopoRouter::ConvertGeometry()
{
	 if(pcb()) {
		qDebug() << "Clearing the list of used pads";
		UsedPadList = new QList<PadType*>;
		PCB->Data->Element=NULL;
// 		PCB->MaxWidth=pcb()->structure().collect();

		qDebug() << "Feeding our SPECTRA geometry to the gEDA-format PCB";

		QList<CSpecctraObject*> Places = pcb()->collect("place");
		QList<CSpecctraObject*> Nets = pcb()->collect("net");
		
		ElementType* newElement;
		RatType *newRat;
		PadType* newPad;

		CPcbPlace* Place;
		CPcbShape* Shape;
		CPcbPin* Pin;
		CPcbNet* Rat;

		int layer;
		qreal x1,x2,y1,y2;
		
		PCB->Data->Rat=NULL;
		for(int NetNum=0; NetNum < Nets.count(); NetNum ++) {
			newRat = new RatType();
			Rat = (CPcbNet*)Nets.at(NetNum);
			QString NetName = Rat->name();
			NetName.replace('"',"");
			qDebug() << __FUNCTION__ << " Rat name: " << NetName;
			Rat->shape().boundingRect().getCoords(&x1,&y1,&x2,&y2);
			newRat->Point1.X=x1;
			newRat->Point1.Y=y1;
			newRat->Point2.X=x2;
			newRat->Point2.Y=y2;
			PCB->Data->Rat=g_list_append(PCB->Data->Rat,newRat);
		}

		for(int PlaceNum=0; PlaceNum < Places.count(); PlaceNum ++) {
			newElement = (ElementType*)g_malloc(sizeof(ElementType));
			Place = (CPcbPlace*)Places.at(PlaceNum);

			qDebug() << __FUNCTION__ << " Unit name: " << Place->unit();

			newElement->Pin=NULL; // First entry of Pin-List NULL, now we fill it.
			for(int PadNum = 0; PadNum < Place->pads(); PadNum ++) {
				Pin = Place->pin(Place->pad(PadNum)->pinRef());

				QString PadName = "";
				PadName+=Place->unit();
				PadName+="-";
				PadName+=Place->pad(PadNum)->pinRef();

				qDebug() << " Pin number: " << Place->pad(PadNum)->pinRef();
				if(Place->pad(PadNum)->net())
					qDebug() << " Pin net: " << Place->pad(PadNum)->net()->name();
				else
					qDebug() << " Pin not in a net ";
	
				newElement->Pad=NULL;
				for(int ShapeNum = 0; ShapeNum < Pin->padstack()->shapes(); ShapeNum ++) {
					qDebug() << " Pad Name: " << qPrintable(PadName);
					Shape = Pin->padstack()->shape(ShapeNum);
					qDebug() << " on layer: " << Shape->layer();
					layer=FindLayer(Shape->layer());
					Shape->shape().boundingRect().getCoords(&x1, &y1, &x2, &y2);
					newPad = CreatePad(PadName, x1, y1, x2, y2);
					newElement->Pad=g_list_append(newElement->Pad,newPad);
				}
				newElement->Pin=g_list_append(newElement->Pin,
					CreatePin(
						(Place->pad(PadNum)->net())?(Place->pad(PadNum)->net()->name()):"none",
						Place->pad(PadNum)->pinRef(),
						Pin->x(),
						Pin->y(),
						SQUAREFLAG
					)
				);
			}
			PCB->Data->Element=g_list_append(PCB->Data->Element,newElement);
		}
		
		/**
		*	We are recently not importing ANY groups
		*	If you're pissed by this fact. Sacrifice some marks yourself and implement
		*	it instead doing homeworks for lecture like me right now... >.<
		*/
		for(int group=0;group<max_group;group++) {
			PCB->LayerGroups.Number[group]=1;
			PCB->LayerGroups.Entries[group][0]=group;
// 			PCB->Data->Layer[group]=(LayerType*)g_malloc(sizeof(LayerType));
			PCB->Data->Layer[group].Line=NULL;
		}
		
		// We don't have vias yet...
		PCB->Data->Via=NULL;

		// Import geometry
		import_geometry(TopoRouterHandle);
	}
}

/**
  * @brief route the current net.
  */
void TopoRouter::route()
{
    if(pcb())
    {
        qDebug() << "Starging routing the net";
		ConvertGeometry();
//         getPads();
//         getNets();
        qDebug() << "\tNumber of nets: " << TopoRouterHandle->netlists->len;
//         toporoute(TopoRouterHandle);
//         hybrid_router(TopoRouterHandle);
//         setState(Idle);
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

