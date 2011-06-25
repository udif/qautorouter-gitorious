/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef SIMPLEROUTER_H
#define SIMPLEROUTER_H

#include <QObject>
#include <QString>

#include <cplugininterface.h>

class CPcb;
class SimpleRouter : public QObject, public CPluginInterface
 {
	Q_OBJECT
	Q_INTERFACES(CPluginInterface)

	public:

		virtual tPluginType type();
		virtual QString title() const;
		virtual QString version() const;
		virtual QString author() const;
		virtual QString website() const;
		virtual QString description() const;

		virtual bool initialize(CPcb* pcb);			/** initialize, gets' called once prior to exec() being called */
		virtual bool exec();						/** get's called repeatedly while exec() returns true, return false to stop */
	protected:
		CPcb*		pcb() {return mPcb;}
	private:
		CPcb*		mPcb;
};

#endif // SIMPLEROUTER_H
