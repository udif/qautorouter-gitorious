/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "qautorouter.h"
#include "cspecctrareader.h"
#include "cpcbstructure.h"
#include "cgpadstack.h"
#include "cpcbnetwork.h"
#include "cpcbnet.h"
#include "cpcb.h"
#include "cpcblayer.h"

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QListWidget>
#include <QListWidgetItem>
#include <QColorDialog>

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
	QObject::connect(preferences->layerColors,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(layerColorDoubleClicked(QModelIndex)));
	this->setWindowIcon(QIcon(":/icons/qautorouter.png"));
	this->setWindowTitle("QAutoRouter "+QString(VERSION_STRING));
	mTimer = startTimer(2000);
}

QAutoRouter::~QAutoRouter()
{
	if ( mRoot != NULL )
		delete mRoot;
	delete ui;
	delete preferences;
}

CPcb* QAutoRouter::pcb()
{
	CSpecctraObject* obj = root();
	if ( obj != NULL && obj->objectClass()=="pcb")
	{
		return (CPcb*)root();
	}
	return NULL;
}

void QAutoRouter::closeEvent(QCloseEvent* e)
{
	if (maybeSave())
	{
		writeSettings();
		e->accept();
	}
	else
	{
		e->ignore();
	}
}

void QAutoRouter::wheelEvent(QWheelEvent * e)
{
	int numDegrees = e->delta() / 8;
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
	e->accept();
}

void QAutoRouter::layerColorDoubleClicked(QModelIndex idx)
{
	QListWidgetItem* item = preferences->layerColors->currentItem();
	QColorDialog dialog;
	dialog.setCurrentColor(item->backgroundColor());
	if ( dialog.exec())
	{
		item->setBackgroundColor(dialog.selectedColor());
		for (int i = 0; i < pcb()->structure()->layers(); ++i)
		{
			if ( item->text() == pcb()->structure()->layer(i)->description() )
			{
				pcb()->structure()->layer(i)->setColor(dialog.selectedColor());
				writeSettings();
				break;
			}
		}
	}
	CSpecctraObject::scene()->update();
}

void QAutoRouter::populateLayersForm()
{
	if ( pcb() != NULL )
	{
		preferences->layerColors->clear();
		for (int i = 0; i < pcb()->structure()->layers(); ++i)
		{
			QListWidgetItem* item = new QListWidgetItem(preferences->layerColors);
			item->setBackground(pcb()->structure()->layer(i)->color());
			item->setText(pcb()->structure()->layer(i)->description());
			preferences->layerColors->addItem(item);
		}
	}
}

void QAutoRouter::editPreferences()
{
	populateLayersForm();
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

	settings.beginGroup("layers");
	if ( pcb()!=NULL)
	{
		for (int i = 0; i < pcb()->structure()->layers(); ++i)
		{
			QString layerName = pcb()->structure()->layer(i)->name();
			settings.setValue(layerName, pcb()->structure()->layer(i)->toBytes());
		}
	}
	settings.endGroup();
}

void QAutoRouter::readSettings()
{
	QSettings settings("8bit.zapto.org", "QAutoRouter");

	settings.beginGroup("MainWindow");
		resize(settings.value("size", QSize(800, 600)).toSize());
		move(settings.value("pos", QPoint(100, 100)).toPoint());
	settings.endGroup();

	settings.beginGroup("layers");
	if ( pcb()!=NULL)
	{
		for (int i = 0; i < pcb()->structure()->layers(); ++i)
		{
			QString layerName = pcb()->structure()->layer(i)->name();
			QByteArray bytes = settings.value(layerName).toByteArray();
			if ( bytes.count() )
			{
				pcb()->structure()->layer(i)->fromBytes(bytes);
			}
		}
	}
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
	ui->actionSave->setIcon(QIcon(":/icons/filesave.png"));
	ui->actionSave_As->setIcon(QIcon(":/icons/filesaveas.png"));
	ui->actionQuit->setIcon(QIcon(":/icons/exit.png"));
	ui->actionZoom_Fit->setIcon(QIcon(":/icons/viewmagfit.png"));
	ui->actionZoom_In->setIcon(QIcon(":/icons/viewmag+.png"));
	ui->actionZoom_Out->setIcon(QIcon(":/icons/viewmag-.png"));
	ui->actionOptions->setIcon(QIcon(":/icons/configure.png"));
	ui->actionAbout->setIcon(QIcon(":/icons/qautorouter.png"));

	ui->actionQuit->setShortcut((QKeySequence(tr("Ctrl+Q"))));
	ui->actionOpen->setShortcut((QKeySequence(tr("Ctrl+O"))));
	ui->actionSave->setShortcut((QKeySequence(tr("Ctrl+S"))));
	ui->actionZoom_In->setShortcut((QKeySequence(tr("Ctrl++"))));
	ui->actionZoom_Out->setShortcut((QKeySequence(tr("Ctrl+-"))));
	ui->actionZoom_Fit->setShortcut((QKeySequence(tr("Ctrl+0"))));

	QObject::connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
	QObject::connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
	QObject::connect(ui->actionSave_As,SIGNAL(triggered()),this,SLOT(saveAs()));
	QObject::connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(close()));
	QObject::connect(ui->actionZoom_In,SIGNAL(triggered()),this,SLOT(zoomIn()));
	QObject::connect(ui->actionZoom_Out,SIGNAL(triggered()),this,SLOT(zoomOut()));
	QObject::connect(ui->actionZoom_Fit,SIGNAL(triggered()),this,SLOT(zoomFit()));
	QObject::connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(editPreferences()));
	QObject::connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));

	QToolBar* file = addToolBar("File");
	file->addAction(ui->actionOpen);
	file->addAction(ui->actionSave);
	file->addAction(ui->actionSave_As);
	file->addAction(ui->actionQuit);

	QToolBar* view = addToolBar("View");
	view->addAction(ui->actionZoom_In);
	view->addAction(ui->actionZoom_Out);
	view->addAction(ui->actionZoom_Fit);

	QToolBar* settings = addToolBar("Settings");
	settings->addAction(ui->actionOptions);

	QToolBar* help = addToolBar("Help");
	help->addAction(ui->actionAbout);
}

void QAutoRouter::clear()
{
	if ( mRoot != NULL )
	{
		delete mRoot;
		mRoot = NULL;
	}
	CGPadstack::clear();
}

bool QAutoRouter::load(QFile& file)
{
	bool rc=false;
	if ( file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		clear();
		CSpecctraReader reader(in,this);
		mRoot = reader.root();
		root()->dump(); /** DEBUG */
		if ( root()->objectClass() == "pcb" )
		{
			readSettings();
			zoomFit();
			rc=true;
		}
		else
		{
			emit fault("root class 'pcb'' expected.");
			clear();
			rc=false;
		}
		file.close();
	}
	return rc;
}

bool QAutoRouter::save()
{
	return saveAs();
}

bool QAutoRouter::saveAs()
{
	QString filename = mFileName;
	filename.replace(".dsn",".ses");
	if (pcb()!=NULL)
	{
		QFileDialog dialog(this,"Save",QDir::currentPath(),tr("Specctra Session (*.ses)"));
		dialog.selectFile(filename);
		if ( dialog.exec() && dialog.selectedFiles().count())
		{
			QFile file(dialog.selectedFiles().at(0));
			if ( file.open(QIODevice::ReadWrite))
			{
				file.write(pcb()->toText(0).toAscii());
				file.close();
				return true;
			}
			else
			{
				emit fault("error saving '"+mFileName+"'");
				return false;
			}
		}
	}
}

void QAutoRouter::open()
{
	if ( maybeSave() )
	{
		QFileDialog dialog(this,"Open",QDir::currentPath(),tr("Specctra Files (*.dsn)"));
		if (dialog.exec())
		{
			mFileName = dialog.selectedFiles().at(0);
			QFile file(mFileName);
			if ( !load(file) )
			{
				emit fault("load file failed.");
			}
		}
	}
}

bool QAutoRouter::maybeSave()
{
	if (CGPadstack::padstacks().count())
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("QAutoRouter"),
									tr("The document has been modified.\n"
									"Do you want to save your changes?"),
									QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void QAutoRouter::updateZoom()
{
	ui->graphicsView->scale(zoom(),zoom());
	CSpecctraObject::scene()->update();
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
	QMessageBox::about (this, "QAutoRouter V0.0", "QAutoRouter "+QString(VERSION_STRING)+" "
													"(c) 2011 Pike Aerospace Research Corporation\n"
													"\n"
													"Mike Sharkey <mike@pikeaero.com>\n"
													"http://8bit.zapto.org\n"
													"\n"
													"This program is free software; you can redistribute it and/or modify\n"
													"it under the terms of the GNU General Public License as published\n"
													"by the Free Software Foundation; either version 2 of the License,\n"
													"or (at your option) any later version. \n"
													"\n"
													"This program is distributed in the hope that it will be useful, \n"
													"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
													"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the \n"
													"GNU General Public License for more details. \n"
													"\n"
													"You should have received a copy of the GNU General Public License \n"
													"along with this program; if not, write to the \n"
													"\n"
													"Free Software Foundation, Inc.,\n"
													"59 Temple Place - Suite 330,\nBoston, MA 02111-1307,\nU.S.A.\n");
}

void QAutoRouter::faultHandler(QString txt)
{
	QMessageBox::information(this,"Information",txt);
}

void QAutoRouter::timerEvent(QTimerEvent* e)
{
	if ( e->timerId() == mTimer )
	{
		if (pcb() != NULL )
		{
			CPcbNetwork* network = pcb()->network();
			if ( network!=NULL )
			{
				statusBar()->showMessage( tr("Nets: ")+QString::number(network->nets()) + " " + tr("Routed: ")+QString::number(network->routed()));
			}
		}
	}
}
