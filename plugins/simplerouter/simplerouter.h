/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef SIMPLEROUTER_H
#define SIMPLEROUTER_H


#include <QObject>
#include <QString>

#include <cplugininterface.h>

class SimpleRouter : public QObject, public CPluginInterface
 {
	Q_OBJECT
	Q_INTERFACES(CPluginInterface)

	public:
		virtual QString title() const;
		virtual QString version() const;
		virtual QString author() const;
		virtual QString website() const;
		virtual QString description() const;
 };

#endif // SIMPLEROUTER_H
