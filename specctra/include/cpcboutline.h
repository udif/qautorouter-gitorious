/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CPCBOUTLINE_H
#define CPCBOUTLINE_H

#include <QObject>
#include <QString>

#include "cspecctraobject.h"

class CPcbPath;
class CPcbOutline : public CSpecctraObject
{
	Q_OBJECT
	public:
		CPcbOutline(QGraphicsItem *parent = 0);
		virtual ~CPcbOutline();

        int                             paths();
        CPcbPath*                       path(int idx);
        CPcbPath*                       path();

};

#endif // CPCBOUTLINE_H

