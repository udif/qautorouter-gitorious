/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "qautorouter.h"
#include "cspecctrareader.h"

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "ui_qautorouter.h"
#include "ui_preferences.h"

QAutoRouter::QAutoRouter(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::QAutoRouter)
, preferences(new Ui::preferences)
, mRoot(NULL)
, mZoom(0.125/2)
{
	preferences->setupUi(&mPreferencesDialog);
	ui->setupUi(this);
	ui->graphicsView->setBackgroundRole(QPalette::Dark);
	ui->graphicsView->setScene(CSpecctraObject::scene());
	setupActions();
	readSettings();
	QObject::connect(this,SIGNAL(fault(QString)),this,SLOT(faultHandler(QString)));
	this->setWindowIcon(QIcon(":/icons/qautorouter.png"));
	this->setWindowTitle("QAutoRouter - V0.0");
}

QAutoRouter::~QAutoRouter()
{
	if ( mRoot != NULL )
		delete mRoot;
    delete ui;
	delete preferences;
}

void QAutoRouter::closeEvent(QCloseEvent* e)
{
	writeSettings();
}

void QAutoRouter::wheelEvent ( QWheelEvent * event )
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	if ( numSteps > 0 )
	{
		for ( int n=0; n < numSteps; n++)
		{
			zoomIn();
		}
	}
	else
	{
		for ( int n=0; n > numSteps; n--)
		{
			zoomOut();
		}
	}
	event->accept();
}

void QAutoRouter::editPreferences()
{
	if ( mPreferencesDialog.exec() == QDialog::Accepted )
	{
		writeSettings();
	}
}

void QAutoRouter::writeSettings()
{
	QSettings settings("8bit.zapto.org", "QAutoRouter");

	settings.beginGroup("MainWindow");
		settings.setValue("size", size());
		settings.setValue("pos", pos());
	settings.endGroup();
}

void QAutoRouter::readSettings()
{
	QSettings settings("8bit.zapto.org", "QAutoRouter");

	settings.beginGroup("MainWindow");
		resize(settings.value("size", QSize(800, 600)).toSize());
		move(settings.value("pos", QPoint(100, 100)).toPoint());
	settings.endGroup();
}


void QAutoRouter::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QAutoRouter::setupActions()
{
	ui->actionOpen->setIcon(QIcon(":/icons/fileopen.png"));
	ui->actionQuit->setIcon(QIcon(":/icons/exit.png"));
	ui->actionOptions->setIcon(QIcon(":/icons/configure.png"));
	ui->actionZoom_Fit->setIcon(QIcon(":/icons/viewmagfit.png"));
	ui->actionZoom_In->setIcon(QIcon(":/icons/viewmag-.png"));
	ui->actionZoom_Out->setIcon(QIcon(":/icons/viewmag+.png"));
	ui->actionAbout->setIcon(QIcon(":/icons/qautorouter.png"));

	ui->actionQuit->setShortcut((QKeySequence(tr("Ctrl+Q"))));
	ui->actionOpen->setShortcut((QKeySequence(tr("Ctrl+O"))));
	ui->actionZoom_In->setShortcut((QKeySequence(tr("Ctrl++"))));
	ui->actionZoom_Out->setShortcut((QKeySequence(tr("Ctrl+-"))));
	ui->actionZoom_Fit->setShortcut((QKeySequence(tr("Ctrl+0"))));

	QObject::connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(close()));
	QObject::connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
	QObject::connect(ui->actionZoom_In,SIGNAL(triggered()),this,SLOT(zoomIn()));
	QObject::connect(ui->actionZoom_Out,SIGNAL(triggered()),this,SLOT(zoomOut()));
	QObject::connect(ui->actionZoom_Fit,SIGNAL(triggered()),this,SLOT(zoomFit()));
	QObject::connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(editPreferences()));
	QObject::connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
}


bool QAutoRouter::load(QFile& file)
{
	bool rc=false;
	if ( file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		if ( mRoot != NULL )
		{
			delete mRoot;
			mRoot = NULL;
		}
		CSpecctraReader reader(in,this);
		mRoot = reader.root();
		root()->dump(); /** DEBUG */
		if ( root()->objectClass() == "pcb" )
		{
			zoomFit();
			rc=true;
		}
		else
			emit fault("root class 'pcb'' expected.");
		file.close();
	}
	return rc;
}

void QAutoRouter::open()
{
	QString fileName;
	QFileDialog dialog(this,"Open",QDir::currentPath(),tr("Specctra Files (*.dsn)"));
	if (dialog.exec())
	{
		QFile file(dialog.selectedFiles().at(0));
		if ( !load(file) )
		{
			emit fault("load file failed.");
		}
	}
}

void QAutoRouter::updateZoom()
{
	ui->graphicsView->scale(zoom(),zoom());
}

void QAutoRouter::zoomIn()
{
	mZoom = 1.125;
	updateZoom();
}

void QAutoRouter::zoomOut()
{
	mZoom = 1.0-0.125;
	updateZoom();
}

void QAutoRouter::zoomFit()
{
	QRectF bounds = CSpecctraObject::scene()->itemsBoundingRect();
	ui->graphicsView->fitInView(bounds,Qt::KeepAspectRatio);
}

void QAutoRouter::about()
{
	QMessageBox::about (this, "QAutoRouter V0.0", tr("QAutoRouter V0.0 (C) 2011 Mike Sharkey <mike@pikeaero.com>"));
}

void QAutoRouter::faultHandler(QString txt)
{
	QMessageBox::information(this,"Information",txt);
}
