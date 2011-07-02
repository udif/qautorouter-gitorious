/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cutil.h"

#include <QStringList>

/**
  * @param tm elapsed time in seconds.
  * @return elased time in "hh:mm:ss" format.
  */
QString CUtil::elapsed(int tm)
{
	QString rc;
	int hour,min,sec;
	hour=tm/3600;
	tm=tm%3600;
	min=tm/60;
	tm=tm%60;
	sec=tm;
	rc.sprintf("%02d:%02d:%02d",hour,min,sec);
	return rc;
}

/**
  * @return the UNIT portion of a UNIT-PIN formatted reference string.
  */
QString CUtil::unitRef(QString unitPinRef)
{
	QString rc;
	QStringList parts = unitPinRef.split("-");
	if ( parts.count() == 2 )
	{
		rc = parts.at(0);
	}
	return rc;
}

/**
  * @return the PIN portion of a UNIT-PIN formatted reference string.
  */
QString CUtil::pinRef(QString unitPinRef)
{
	QString rc;
	QStringList parts = unitPinRef.split("-");
	if ( parts.count() == 2 )
	{
		rc = parts.at(1);
	}
	return rc;
}

