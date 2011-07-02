/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CUTIL_H
#define CUTIL_H

#include <QString>

class CUtil
{
	public:
		static QString				elapsed(int tm);
		static QString				unitRef(QString unitPinRef);
		static QString				pinRef(QString unitPinRef);
};

#endif // CUTIL_H
