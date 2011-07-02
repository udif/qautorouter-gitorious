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
#include "cpcbclass.h"

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QListWidget>
#include <QListWidgetItem>
#include <QColorDialog>
#include <QPluginLoader>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "ui_qautorouter.h"
#include "ui_preferences.h"
#include "ui_layerpreferences.h"

#include "cplugininterface.h"

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
	ui->graphicsView->setScene(CSpecctraObject::scene());
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

/**
  * @brief A layer color selection button was clicked, reset the color for the selected layer
  */
void QAutoRouter::layerColorClicked()
{
	QListWidgetItem* item = preferences->layerList->currentItem();
	QColorDialog dialog;
	dialog.setCurrentColor(item->backgroundColor());
	if ( dialog.exec())
	{
		QPalette palette = layerpreferences->colorButton->palette();
		palette.setColor(QPalette::Button,dialog.selectedColor());
		palette.setColor(QPalette::Background,dialog.selectedColor());
		layerpreferences->colorButton->setPalette(palette);
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

/**
  * @brief A layer was clicked.
  */
void QAutoRouter::layerClicked(QModelIndex /* idx */)
{
	QListWidgetItem* item = preferences->layerList->currentItem();
	if ( item != NULL && pcb()!=NULL && pcb()->structure()!=NULL )
	{
		CPcbLayer* layer = pcb()->structure()->layer(item->data(Qt::UserRole).toString());
		if ( layer != NULL )
		{
			QPalette palette = layerpreferences->colorButton->palette();
			palette.setColor(QPalette::Button,item->backgroundColor());
			palette.setColor(QPalette::Background,item->backgroundColor());
			layerpreferences->colorButton->setPalette(palette);
			if ( layer->direction() == CPcbLayer::Horizontal)
				layerpreferences->radioHorizontal->setChecked(true);
			else
				layerpreferences->radioVertical->setChecked(true);
			if ( mLayerPreferencesDialog.exec() )
			{
				palette.setColor(QPalette::Button,item->backgroundColor());
				palette.setColor(QPalette::Background,item->backgroundColor());
				layerpreferences->colorButton->setPalette(palette);
				if ( layerpreferences->radioHorizontal->isChecked() )
					layer->setDirection(CPcbLayer::Horizontal);
				else
					layer->setDirection(CPcbLayer::Vertical);
				populateLayersForm(); /* redraw the layers for with new descriptions */
			}
			preferences->layerList->setCurrentRow(-1);
		}
		else
		{
			emit fault(tr("problem loading layer '")+item->data(Qt::UserRole).toString()+"'");
		}
	}
	else
	{
		emit fault(tr("problem with layer."));
	}
}

/**
  * @brief Populate layers form with layers descriptions.
  */
void QAutoRouter::populateLayersForm()
{
	if ( pcb() != NULL && pcb()->structure()!=NULL )
	{
		preferences->layerList->clear();
		for (int i = 0; i < pcb()->structure()->layers(); ++i)
		{
			QListWidgetItem* item = new QListWidgetItem(preferences->layerList);
			item->setBackground(pcb()->structure()->layer(i)->color());
			item->setText(pcb()->structure()->layer(i)->description());
			item->setData(Qt::UserRole,pcb()->structure()->layer(i)->name());
			preferences->layerList->addItem(item);
		}
	}
}

void QAutoRouter::netsClicked(QModelIndex /* idx */)
{
	/** FIXME - get here on a nets click - open net class picker. (embed ComboBox in the nets tree?) */
}

/**
  * @brief populate the nets form.
  */
void QAutoRouter::populateNetsForm()
{
	if ( pcb()!=NULL && pcb()->network()!=NULL )
	{
		preferences->netsTree->clear();
		for(int i = 0; i < pcb()->network()->nets(); i++)
		{
			CPcbNet* net = pcb()->network()->net(i);
			QTreeWidgetItem *netsItem = new QTreeWidgetItem(preferences->netsTree);
			netsItem->setText(0, net->name());
			netsItem->setData(0,Qt::UserRole,net->name());
			QTreeWidgetItem *netsClass = new QTreeWidgetItem(netsItem);
			netsClass->setText(0,net->netClass()->name());
			netsClass->setData(0,Qt::UserRole,net->netClass()->name());
			netsClass->setText(1,QString::number(net->netClass()->width())+"mil");
			netsClass->setData(1,Qt::UserRole,net->netClass()->name());
			preferences->netsTree->addTopLevelItem(netsItem);
		}
		preferences->netsTree->sortItems(0,Qt::AscendingOrder);
		preferences->netsTree->resizeColumnToContents(0);
	}
}

/**
  * @brief populate the net classes properties form.
  */
void QAutoRouter::populateNetClassesForm()
{
	if ( pcb()!=NULL && pcb()->network()!=NULL )
	{
		preferences->netClassesTable->clear();
		preferences->netClassesTable->setColumnCount(2);
		preferences->netClassesTable->setHorizontalHeaderItem(0,new QTableWidgetItem("Class"));
		preferences->netClassesTable->setHorizontalHeaderItem(1,new QTableWidgetItem("Width (mil)"));
		for(int i = 0; i < pcb()->network()->netClasses(); i++)
		{
			preferences->netClassesTable->setRowCount(i+1);
			CPcbClass* netClass = pcb()->network()->netClass(i);
			preferences->netClassesTable->setItem(i,0,new QTableWidgetItem(netClass->name()));
			preferences->netClassesTable->setItem(i,1,new QTableWidgetItem(QString::number(netClass->width())));
		}
		preferences->netClassesTable->resizeColumnsToContents();
	}
}

/**
  * @brief new net class
  */
void QAutoRouter::newNetClass()
{
	preferences->netClassesTable->setRowCount(preferences->netClassesTable->rowCount()+1);
	/** FIXME - instantiate a new PcbClass object and add to the Pcb object tree. */
}

/**
  * @brief elete new class
  */
void QAutoRouter::deleteNetClass()
{
	/** FIXME delete from the table and from the Pcb object tree - do we need to have a *write protect* on original sourced Pcb objects? */
}

/**
  * @brief present the preferences dialog and save the changes if requested
  */
void QAutoRouter::editPreferences()
{
	populateLayersForm();
	populateNetsForm();
	populateNetClassesForm();
	if ( mPreferencesDialog.exec() == QDialog::Accepted )
	{
		writeSettings();
	}
}

/**
  * @return the current release version
  */
QString QAutoRouter::version()
{
	return VERSION_STRING;
}

/**
  * @brief store settings and preferences.
  */
void QAutoRouter::writeSettings()
{
	QSettings settings("8bit.zapto.org", "QAutoRouter");

	settings.beginGroup("MainWindow");
		settings.setValue("size", size());
		settings.setValue("pos", pos());
		settings.setValue("version",version());
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

	settings.beginWriteArray("plugins");
	for (int n = 0; n < preferences->pluginTree->topLevelItemCount(); ++n) {
		settings.setArrayIndex(n);
		QTreeWidgetItem* item = preferences->pluginTree->topLevelItem(n);
		QString filename = item->data(0,Qt::UserRole).toString();
		settings.setValue("plugin",filename);
	}
	settings.endArray();

	settings.setValue("defaultRouter", preferences->routerCombo->currentIndex());
	settings.setValue("defaultPostRouter", preferences->postRouterCombo->currentIndex());
}

/**
  * @brief load settings and preferences.
  */
void QAutoRouter::readSettings()
{
	QSettings settings("8bit.zapto.org", "QAutoRouter");

	settings.beginGroup("MainWindow");
		resize(settings.value("size", QSize(800, 600)).toSize());
		move(settings.value("pos", QPoint(100, 100)).toPoint());
		QString settingsVersion = settings.value("version",version()).toString();
		if ( settingsVersion != version() )
		{
			QMessageBox::information(this,tr("Version Mismatch"),tr("Settings version ")+settingsVersion+tr(" does not match current version ")+version());
		}
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

	int size = settings.beginReadArray("plugins");
	for (int n = 0; n < size; ++n) {
		settings.setArrayIndex(n);
		QString filename = settings.value("plugin").toString();
		QString errorString;
		if ( !loadPlugin(filename,errorString))
		{
			QMessageBox::warning(this,tr("Plugin Loader"),tr("Load '")+filename+tr("' failed: ")+errorString);
		}
	}
	settings.endArray();

	preferences->routerCombo->setCurrentIndex( settings.value("defaultRouter",-1).toInt());
	preferences->postRouterCombo->setCurrentIndex(settings.value("defaultPostRouter",-1).toInt());
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
  * @brief called exactly once to setup UI actions.
  */
void QAutoRouter::setupActions()
{
	ui->actionOpen->setIcon(QIcon(":/icons/fileopen.png"));
	ui->actionSave->setIcon(QIcon(":/icons/filesave.png"));
	ui->actionSave_As->setIcon(QIcon(":/icons/filesaveas.png"));
	ui->actionQuit->setIcon(QIcon(":/icons/exit.png"));
	ui->actionZoom_Fit->setIcon(QIcon(":/icons/viewmagfit.png"));
	ui->actionZoom_In->setIcon(QIcon(":/icons/viewmag+.png"));
	ui->actionZoom_Out->setIcon(QIcon(":/icons/viewmag-.png"));
	ui->actionStart->setIcon(QIcon(":/icons/player_play.png"));
	ui->actionStop->setIcon(QIcon(":/icons/player_stop.png"));
	ui->actionOptions->setIcon(QIcon(":/icons/configure.png"));
	ui->actionAbout->setIcon(QIcon(":/icons/qautorouter.png"));

	ui->actionQuit->setShortcut(QKeySequence(QKeySequence::Quit));
	ui->actionOpen->setShortcut(QKeySequence(QKeySequence::Open));
	ui->actionSave->setShortcut(QKeySequence(QKeySequence::Save));
	ui->actionZoom_In->setShortcut(QKeySequence(QKeySequence::ZoomIn));
	ui->actionZoom_Out->setShortcut(QKeySequence(QKeySequence::ZoomOut));
	ui->actionZoom_Fit->setShortcut(QKeySequence(Qt::Key_F3));
	ui->actionStart->setShortcut(QKeySequence(Qt::Key_F7));
	ui->actionStop->setShortcut(QKeySequence(Qt::Key_Escape));

	QObject::connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
	QObject::connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
	QObject::connect(ui->actionSave_As,SIGNAL(triggered()),this,SLOT(saveAs()));
	QObject::connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(close()));
	QObject::connect(ui->actionZoom_In,SIGNAL(triggered()),this,SLOT(zoomIn()));
	QObject::connect(ui->actionZoom_Out,SIGNAL(triggered()),this,SLOT(zoomOut()));
	QObject::connect(ui->actionZoom_Fit,SIGNAL(triggered()),this,SLOT(zoomFit()));
	QObject::connect(ui->actionStart,SIGNAL(triggered()),this,SLOT(start()));
	QObject::connect(ui->actionStop,SIGNAL(triggered()),this,SLOT(stop()));
	QObject::connect(ui->actionOptions,SIGNAL(triggered()),this,SLOT(editPreferences()));
	QObject::connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));

	QToolBar* file = addToolBar(tr("File"));
	file->addAction(ui->actionOpen);
	file->addAction(ui->actionSave);
	file->addAction(ui->actionSave_As);
	file->addAction(ui->actionQuit);

	QToolBar* view = addToolBar(tr("View"));
	view->addAction(ui->actionZoom_In);
	view->addAction(ui->actionZoom_Out);
	view->addAction(ui->actionZoom_Fit);

	QToolBar* action = addToolBar(tr("Action"));
	action->addAction(ui->actionStart);
	action->addAction(ui->actionStop);

	QToolBar* settings = addToolBar(tr("Settings"));
	settings->addAction(ui->actionOptions);

	QToolBar* help = addToolBar(("Help"));
	help->addAction(ui->actionAbout);

	ui->actionStop->setEnabled(false);
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
	CGPadstack::clear();
}

/**
  * @return a pointer to the currently running auto-router plugin.
  */
CPluginInterface* QAutoRouter::autorouter()
{
	return mAutoRouter;
}

/**
  * @brief start routing
  */
void QAutoRouter::start()
{
	if ( pcb() != NULL )
	{
		int idx = preferences->routerCombo->currentIndex();
		if ( idx >= 0 )
		{
			QString filename = preferences->routerCombo->itemData(idx).toString();
			mPluginLoader.setFileName(filename);
			if ( mPluginLoader.load() )
			{
				QObject* plugin = mPluginLoader.instance();
				if ( plugin != NULL )
				{
					ui->actionStart->setEnabled(false);
					ui->actionStop->setEnabled(true);
					mAutoRouter = qobject_cast<CPluginInterface *>(plugin);
					if (autorouter() != NULL)
					{
						QEventLoop loop;
						ui->actionStart->setEnabled(false);
						autorouter()->start(pcb());
						while ( autorouter() != NULL && autorouter()->exec() )
						{
							loop.processEvents();
						}
						QMessageBox::information(this,tr("Routing Complete"),tr("Routing Completed. Total time: ")+autorouter()->elapsed());
					}
					mAutoRouter=NULL;
					ui->actionStart->setEnabled(true);
					ui->actionStop->setEnabled(false);
				}
			}
		}
	}
	else
	{
		emit fault(tr("PCB is NULL"));
	}
}

/**
  * @brief stop routing
  */
void QAutoRouter::stop()
{
	if (autorouter()!=NULL)
	{
		autorouter()->stop();
	}
}

/**
  * @brief load a file by filename
  */
bool QAutoRouter::load(QFile& file)
{
	bool rc=false;
	stop();
	if ( file.open(QIODevice::ReadOnly))
	{
		QTextStream in(&file);
		clear();
		CSpecctraReader reader(in,this);
		mRoot = reader.root();
		if ( root() != NULL )
		{
			root()->dump(); /** DEBUG */
			if ( root()->objectClass() == "pcb" )
			{
				QObject::connect(pcb(),SIGNAL(status(QString)),this,SLOT(status(QString)));
				readSettings();
				populateLayersForm();
				populateNetsForm();
				populateNetClassesForm();
				zoomFit();
				rc=true;
			}
			else
			{
				emit fault(tr("root class 'pcb'' expected."));
				clear();
				rc=false;
			}
		}
		else
		{
			emit fault(tr("load problem."));
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
		QFileDialog dialog(this,tr("Save"),QDir::currentPath(),tr("Specctra Session (*.ses)"));
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
				emit fault(tr("error saving '")+mFileName+"'");
				return false;
			}
		}
	}
	return true;
}

/**
  * @brief open a file by dialog
  */
void QAutoRouter::open()
{
	if ( maybeSave() )
	{
		QFileDialog dialog(this,tr("Open"),QDir::currentPath(),tr("Specctra Files (*.dsn)"));
		if (dialog.exec())
		{
			mFileName = dialog.selectedFiles().at(0);
			QFile file(mFileName);
			if ( !load(file) )
			{
				emit fault(tr("load file failed."));
			}
		}
	}
}

/**
  * @brief prior to discarding current work, call this...
  */
bool QAutoRouter::maybeSave()
{
	if (CGPadstack::padstacks().count())
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("QAutoRouter"),tr("The document has been modified.\nDo you want to save your changes?"),QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
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

/**
  * @brief Show a status message
  */
void QAutoRouter::status(QString txt)
{
	this->statusBar()->showMessage(txt);
}

void QAutoRouter::about()
{
	QMessageBox::about (this, "QAutoRouter V0.0", "QAutoRouter "+version()+" "
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
	QMessageBox::information(this,tr("Fault"),txt);
}

void QAutoRouter::timerEvent(QTimerEvent* e)
{
	if ( e->timerId() == mTimer )
	{
		QString msg;
		if ( autorouter()==NULL ) /* running? */
		{
			msg = tr("[Idle] ");
			statusBar()->showMessage( msg );
		}
	}
}

/**
  * @brief Load a plugin by file name.
  */
bool QAutoRouter::loadPlugin(QString filename,QString& errorString)
{
	bool rc=false;
	mPluginLoader.setFileName(filename);
	if ( !mPluginLoader.isLoaded())
	{
		if ( mPluginLoader.load() )
		{
			QObject* plugin = mPluginLoader.instance();
			if ( plugin != NULL )
			{
				CPluginInterface *iPlugin = qobject_cast<CPluginInterface *>(plugin);
				 if (iPlugin)
				{
					bool inTree=false;
					for (int n = 0; n < preferences->pluginTree->topLevelItemCount(); ++n) {
						QTreeWidgetItem* item = preferences->pluginTree->topLevelItem(n);
						QString filename = item->data(0,Qt::UserRole).toString();
						if ( mPluginLoader.fileName() == filename )
						{
							inTree=true;
							break;
						}
					}
					if ( !inTree )
					{
						QString title;
						if (iPlugin->type()==CPluginInterface::RouterPlugin)
						{
							title=tr("[Router] ");
							preferences->routerCombo->addItem(iPlugin->title()+" V"+iPlugin->version(),mPluginLoader.fileName());
						}
						else
						{
							title=tr("[Post-Router] ");
							preferences->postRouterCombo->addItem(iPlugin->title()+" V"+iPlugin->version(),mPluginLoader.fileName());
						}
						title += iPlugin->title();
						QTreeWidgetItem *pluginItem = new QTreeWidgetItem(preferences->pluginTree);
						pluginItem->setText(0, title);
						pluginItem->setData(0,Qt::UserRole,mPluginLoader.fileName());
						QTreeWidgetItem *pluginVersion = new QTreeWidgetItem(pluginItem);
						pluginVersion->setText(0, tr("Version: ")+iPlugin->version());
						QTreeWidgetItem *pluginAuthor = new QTreeWidgetItem(pluginItem);
						pluginAuthor->setText(0, tr("Author: ")+iPlugin->author());
						QTreeWidgetItem *pluginWebsite = new QTreeWidgetItem(pluginItem);
						pluginWebsite->setText(0, tr("Website: ")+iPlugin->website());
						QTreeWidgetItem *pluginDescription = new QTreeWidgetItem(pluginItem);
						pluginDescription->setText(0, tr("About: ")+iPlugin->description());
						preferences->pluginTree->addTopLevelItem(pluginItem);
					}
					rc = true;
				}
			}
		}
	}
	else
	{
		rc=true;
	}
	errorString = mPluginLoader.errorString();
	return rc;
}

/**
  * @brief Produce a plugin loader dialog for selecting a plugin to load, and load the plugin
  */
void QAutoRouter::addPlugin()
{
	QFileDialog dialog(this,tr("Open"),QDir::currentPath(),tr("Plug-In Files (*.so)"));
	if (dialog.exec())
	{
		QString filename = dialog.selectedFiles().at(0);
		QPluginLoader loader(filename);
		QString errorString;
		if ( loadPlugin(filename,errorString) )
		{
			writeSettings();
		}
		else
		{
			QMessageBox::warning(this,tr("Plugin Loader"),tr("Load '")+filename+tr("' failed: ")+errorString);
		}
	}
}

/**
  * @brief Remove a plugin.
  */
void QAutoRouter::removePlugin()
{
	QList<QTreeWidgetItem*> pluginItems = preferences->pluginTree->selectedItems();
	for(int n=0; n < pluginItems.count(); n++)
	{
		QTreeWidgetItem* item = pluginItems.at(n);
		QString filename = item->data(0,Qt::UserRole).toString();
		if ( filename.length() )
		{
			mPluginLoader.setFileName(filename);
			if ( mPluginLoader.unload() )
			{
				preferences->pluginTree->removeItemWidget(item,0);
				writeSettings();
			}
			else
			{
				QMessageBox::warning(this,tr("Plugin Unload"),tr("Unload '")+filename+tr("' failed: ")+mPluginLoader.errorString());
			}
		}
	}
}



