/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "qautorouter-common.h"

QAutoRouter::QAutoRouter(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::QAutoRouter)
, preferences(new Ui::preferences)
, layerpreferences(new Ui::layerpreferences)
, mRoot(NULL)
, mZoom(0.125/2)
, mAutoRouter(NULL)
{
	preferences->setupUi(&mPreferencesDialog);
	layerpreferences->setupUi(&mLayerPreferencesDialog);
	ui->setupUi(this);
	ui->graphicsView->setBackgroundRole(QPalette::Dark);
	ui->graphicsView->setScene(CSpecctraObject::globalScene());
	ui->graphicsView->scale(1, -1); /* flip Y axis */
	ui->graphicsView->setInteractive(true);
	ui->graphicsView->setRubberBandSelectionMode(Qt::IntersectsItemShape);
	ui->graphicsView->setEnabled(true);
	ui->graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	setupActions();
	readSettings();
	QObject::connect(this,SIGNAL(fault(QString)),this,SLOT(faultHandler(QString)));
	QObject::connect(preferences->layerList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(layerClicked(QModelIndex)));
	QObject::connect(preferences->netsTree,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(netsClicked(QModelIndex)));
	QObject::connect(layerpreferences->colorButton,SIGNAL(clicked()),this,SLOT(layerColorClicked()));
	QObject::connect(preferences->addPluginButton,SIGNAL(clicked()),this,SLOT(addPlugin()));
	QObject::connect(preferences->removePluginButton,SIGNAL(clicked()),this,SLOT(removePlugin()));
	QObject::connect(preferences->newNetClassButton,SIGNAL(clicked()),this,SLOT(newNetClass()));
	QObject::connect(preferences->deleteNetClassButton,SIGNAL(clicked()),this,SLOT(deleteNetClass()));
	this->setWindowIcon(QIcon(":/icons/qautorouter.png"));
	this->setWindowTitle("QAutoRouter "+version());
	mTimer = startTimer(2000);

	preferences->pluginTree->setHeaderLabel(tr("Plugins"));

	QStringList netsTreeLabels;
	netsTreeLabels << tr("Net");
	netsTreeLabels << tr("Class Properties");
	preferences->netsTree->setColumnCount(2);
	preferences->netsTree->setHeaderLabels(netsTreeLabels);

}

QAutoRouter::~QAutoRouter()
{
	clear();
	delete ui;
	delete preferences;
}

CPcb* QAutoRouter::pcb()
{
	if ( root() != NULL )
		return root()->pcb();
	return NULL;
}

/**
  * @brief The window has been requested to close.
  */
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

/**
  * @brief The mouse scroll wheel activity has been detected.
  */
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

/**
  * @return the current release version
  */
QString QAutoRouter::version()
{
	return VERSION_STRING;
}

/**
  * @brief Some desktop thing changed...
  */
void QAutoRouter::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type())
	{
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

/**
  * @brief Clear new, as in preparing to load a new file.
  */
void QAutoRouter::clear()
{
	if ( mRoot != NULL )
	{
		delete mRoot;
		mRoot = NULL;
	}
	CSpecctraObject::globalScene()->clear();
}

void QAutoRouter::updateZoom()
{
	ui->graphicsView->scale(zoom(),zoom());
	CSpecctraObject::globalScene()->update();
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
	QRectF bounds;
	if ( pcb() != NULL && pcb()->structure() != NULL && pcb()->structure()->boundary() != NULL )
		bounds = pcb()->structure()->boundary()->boundingRect();
	else
		bounds = CSpecctraObject::globalScene()->itemsBoundingRect();
	ui->graphicsView->centerOn(bounds.center());
	ui->graphicsView->fitInView(bounds,Qt::KeepAspectRatio);
	CSpecctraObject::globalScene()->update();
}

/**
  * @brief Show a status message
  */
void QAutoRouter::status(QString txt)
{
	this->statusBar()->showMessage(txt);
}

void QAutoRouter::about()
{
	QMessageBox::about (this, "About QAutoRouter", "QAutoRouter "+version()+" "
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
	QMessageBox::information(this,tr("Information Message"),txt);
}

void QAutoRouter::timerEvent(QTimerEvent* e)
{
	if ( e->timerId() == mTimer )
	{
		if ( autorouter()==NULL ) /* running? */
		{
			statusBar()->showMessage( tr("[Idle] ") );
		}
	}
}



