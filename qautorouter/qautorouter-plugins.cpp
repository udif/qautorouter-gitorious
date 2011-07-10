/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "qautorouter-common.h"

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
#ifdef Q_OS_WIN32
	QFileDialog dialog(this,tr("Open"),QDir::currentPath(),tr("Plug-In Files (*.dll)"));
#else
	QFileDialog dialog(this,tr("Open"),QDir::currentPath(),tr("Plug-In Files (*.so)"));
#endif
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


