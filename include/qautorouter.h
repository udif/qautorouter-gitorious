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
#include <QPluginLoader>

#include "cpcb.h"

namespace Ui {
    class QAutoRouter;
	class preferences;
	class layerpreferences;
}

#define VERSION_STRING	"V0.0.2"

class CPcb;
class CPluginInterface;
class QAutoRouter : public QMainWindow
{
    Q_OBJECT
	public:
		QAutoRouter(QWidget *parent = 0);
		~QAutoRouter();
		double					zoom() {return mZoom;}
		CSpecctraObject*		root() {return mRoot;}
		CPcb*					pcb();
		QString					version();
	signals:
		void					fault(QString txt);
	public slots:
		void					clear();
		void					updateZoom();
		bool					load(QFile& file);
		void					faultHandler(QString txt);
		void					status(QString txt);

	protected slots:
		void					newNetClass();
		void					deleteNetClass();
		bool					loadPlugin(QString filename,QString& errorString);
		void					addPlugin();
		void					removePlugin();
		void					layerClicked(QModelIndex);
		void					layerColorClicked();
		void					netsClicked(QModelIndex);
		void					readSettings();
		void					writeSettings();
		void					editPreferences();
		void					start();
		void					stop();
		void					open();
		bool					save();
		bool					saveAs();
		void					zoomIn();
		void					zoomOut();
		void					zoomFit();
		void					about();
	protected:
		bool					maybeSave();
		void					populateLayersForm();
		void					populateNetsForm();
		void					populateNetClassesForm();
		void					setupActions();
		void					changeEvent(QEvent *e);
		void					closeEvent(QCloseEvent* e);
		void					wheelEvent ( QWheelEvent * event );
		void					timerEvent(QTimerEvent* e);
		CPluginInterface*		autorouter();

	private:
		Ui::QAutoRouter*		ui;
		Ui::preferences*		preferences;
		Ui::layerpreferences*	layerpreferences;
		QDialog					mPreferencesDialog;
		QDialog					mLayerPreferencesDialog;
		CSpecctraObject*		mRoot;
		double					mZoom;
		int						mTimer;
		QString					mFileName;
		QPluginLoader			mPluginLoader;
		CPluginInterface*		mAutoRouter;
};

#endif // QAUTOROUTER_H
