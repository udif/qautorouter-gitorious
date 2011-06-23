/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef QAUTOROUTER_H
#define QAUTOROUTER_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFile>
#include <QCloseEvent>
#include <QDialog>
#include <QWheelEvent>

#include "cpcb.h"

namespace Ui {
    class QAutoRouter;
	class preferences;
}

#define VERSION_STRING	"V0.0.1"

class CPcb;
class QAutoRouter : public QMainWindow
{
    Q_OBJECT
	public:
		QAutoRouter(QWidget *parent = 0);
		~QAutoRouter();
		double					zoom() {return mZoom;}
		CSpecctraObject*		root() {return mRoot;}
		CPcb*					pcb();
	signals:
		void					fault(QString txt);
	public slots:
		void					clear();
		void					updateZoom();
		bool					load(QFile& file);
		void					faultHandler(QString txt);

	protected slots:
		void					layerColorDoubleClicked(QModelIndex idx);
		void					readSettings();
		void					writeSettings();
		void					editPreferences();
		void					open();
		bool					save();
		void					zoomIn();
		void					zoomOut();
		void					zoomFit();
		void					about();
	protected:
		bool					maybeSave();
		void					populateLayersForm();
		void					setupActions();
		void					changeEvent(QEvent *e);
		void					closeEvent(QCloseEvent* e);
		void					wheelEvent ( QWheelEvent * event );
		void					timerEvent(QTimerEvent* e);

	private:
		Ui::QAutoRouter*		ui;
		Ui::preferences*		preferences;
		double					mZoom;
		QDialog					mPreferencesDialog;
		CSpecctraObject*		mRoot;
		int						mTimer;
};

#endif // QAUTOROUTER_H
