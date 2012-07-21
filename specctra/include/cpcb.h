/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCB_H
#define CPCB_H

#include <QObject>
#include "cspecctraobject.h"

class CPcbStructure;
class CPcbPlacement;
class CPcbLibrary;
class CPcbNetwork;
class CPcb : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcb(QGraphicsItem *parent = 0);
		virtual ~CPcb();

        CPcbStructure*                  structure();
        CPcbPlacement*                  placement();
        CPcbLibrary*                    library();
        CPcbNetwork*                    network();

        /* gEDA export methods */
        virtual QString					toGeda(int lvl=0);
        virtual QString					gedaObjectClass();
        virtual QStringList				gedaProperties();
        virtual QList<CSpecctraObject*>	gedaChildren();

public slots:
        void                            yield();
	signals:
        void                            status(QString txt);
	private:
        CPcbStructure*                  mStructure;
        CPcbPlacement*                  mPlacement;
        CPcbLibrary*                    mLibrary;
        CPcbNetwork*                    mNetwork;
};

#endif // CPCB_H

