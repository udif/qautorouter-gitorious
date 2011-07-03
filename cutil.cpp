/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cutil.h"
#include "cgpadstack.h"
#include "cpcbnet.h"

#include <QStringList>

static QPointF gSortPt; /** point for sorting relative to a point */

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

/**
  * @return a UNIT-PIN string from UNIT and PIN.
  */
QString CUtil::placeRef(QString unitRef, QString pinRef)
{
	QString rc;
	rc = unitRef+"-"+pinRef;
	return rc;
}

/**
  * @return sorted padstack list
  */
static bool comparePadstackPosAscending(CGPadstack* s1,CGPadstack* s2)
{
	QPointF p1 = s1->origin() - gSortPt;
	QPointF p2 = s2->origin() - gSortPt;
	double len1 = p1.manhattanLength();
	double len2 = p2.manhattanLength();
	return len1 < len2;
}
static bool comparePadstackPosDescending(CGPadstack* s1,CGPadstack* s2)
{
	QPointF p1 = s1->origin() - gSortPt;
	QPointF p2 = s2->origin() - gSortPt;
	double len1 = p1.manhattanLength();
	double len2 = p2.manhattanLength();
	return len1 > len2;
}
/**
  * @brief sort a list of padstacks by their position relative to a specified point.
  * @param pads The padstacks to sort.
  * @param pt The point to use as a reference point.
  * @param order The sorting order.
  */
int CUtil::sort(QList<CGPadstack*>& pads,QPointF pt, tSortOrder order)
{
	gSortPt=pt;
	if ( order == CUtil::Ascending)
		qSort(pads.begin(), pads.end(), comparePadstackPosAscending);
	else if ( order == CUtil::Descending)
		qSort(pads.begin(), pads.end(), comparePadstackPosDescending);
	return pads.count();
}


/**
  * @return sorted net list
  */
static bool compareNetsPosAscending(CPcbNet* s1,CPcbNet* s2)
{
	if ( s1->padstacks() > 0 && s2->padstacks() > 0 )
	{
		QPointF p1 = s1->padstack(0)->origin() - gSortPt;
		QPointF p2 = s2->padstack(0)->origin() - gSortPt;
		double len1 = p1.manhattanLength();
		double len2 = p2.manhattanLength();
		return len1 < len2;
	}
	return false;
}
static bool compareNetsPosDescending(CPcbNet* s1,CPcbNet* s2)
{
	if ( s1->padstacks() > 0 && s2->padstacks() > 0 )
	{
		QPointF p1 = s1->padstack(0)->origin() - gSortPt;
		QPointF p2 = s2->padstack(0)->origin() - gSortPt;
		double len1 = p1.manhattanLength();
		double len2 = p2.manhattanLength();
		return len1 > len2;
	}
	return false;
}
/**
  * @brief sort a list of nets by their position relative to a specified point.
  * @param nets The nets to sort.
  * @param pt The point to use as a reference point.
  * @param order The sorting order.
  */
int CUtil::sort(QList<CPcbNet*>& nets,QPointF pt, tSortOrder order)
{
	gSortPt=pt;
	if ( order == CUtil::Ascending)
		qSort(nets.begin(), nets.end(), compareNetsPosAscending);
	else if ( order == CUtil::Descending)
		qSort(nets.begin(), nets.end(), compareNetsPosDescending);
	return nets.count();
}


