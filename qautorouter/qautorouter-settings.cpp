/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "qautorouter-common.h"

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

	settings.setValue("opacityTracks",		preferences->opacityTracks->value());
	settings.setValue("opacityVias",		preferences->opacityVias->value());
	settings.setValue("opacityPads",		preferences->opacityPads->value());
	settings.setValue("opacityPlanes",		preferences->opacityPlanes->value());
	settings.setValue("opacityOutlines",	preferences->opacityOutlines->value());
	settings.setValue("opacityBorders",		preferences->opacityBorders->value());

	settings.setValue("visibilityTracks",	preferences->visibilityTracks->isChecked());
	settings.setValue("visibilityVias",		preferences->visibilityVias->isChecked());
	settings.setValue("visibilityPads",		preferences->visibilityPads->isChecked());
	settings.setValue("visibilityPlanes",	preferences->visibilityPlanes->isChecked());
	settings.setValue("visibilityOutlines",	preferences->visibilityOutlines->isChecked());
	settings.setValue("visibilityBorders",	preferences->visibilityBorders->isChecked());

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
	if ( pcb()!=NULL && pcb()->structure()!=NULL)
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

	preferences->opacityTracks->setValue(settings.value("opacityTracks",0.45).toDouble());
	preferences->opacityVias->setValue(settings.value("opacityVias",0.55).toDouble());
	preferences->opacityPads->setValue(settings.value("opacityPads",0.55).toDouble());
	preferences->opacityPlanes->setValue(settings.value("opacityPlanes",0.25).toDouble());
	preferences->opacityOutlines->setValue(settings.value("opacityOutlines",0.75).toDouble());
	preferences->opacityBorders->setValue(settings.value("opacityBorders",0.75).toDouble());

	preferences->visibilityTracks->setChecked(settings.value("opacityTracks",true).toBool());
	preferences->visibilityVias->setChecked(settings.value("opacityVias",true).toBool());
	preferences->visibilityPads->setChecked(settings.value("opacityPads",true).toBool());
	preferences->visibilityPlanes->setChecked(settings.value("opacityPlanes",true).toBool());
	preferences->visibilityOutlines->setChecked(settings.value("opacityOutlines",true).toBool());
	preferences->visibilityBorders->setChecked(settings.value("opacityBorders",true).toBool());

	CSpecctraObject::globalOpacity()[CSpecctraObject::None]		=	0.5;
	CSpecctraObject::globalOpacity()[CSpecctraObject::Track]	=	preferences->opacityTracks->value();
	CSpecctraObject::globalOpacity()[CSpecctraObject::Via]		=	preferences->opacityVias->value();
	CSpecctraObject::globalOpacity()[CSpecctraObject::Pad]		=	preferences->opacityPads->value();
	CSpecctraObject::globalOpacity()[CSpecctraObject::Plane]	=	preferences->opacityPlanes->value();
	CSpecctraObject::globalOpacity()[CSpecctraObject::Outline]	=	preferences->opacityOutlines->value();
	CSpecctraObject::globalOpacity()[CSpecctraObject::Border]	=	preferences->opacityBorders->value();

	CSpecctraObject::globalVisibility()[CSpecctraObject::None]		=	true;
	CSpecctraObject::globalVisibility()[CSpecctraObject::Track]		=	preferences->visibilityTracks->isChecked();
	CSpecctraObject::globalVisibility()[CSpecctraObject::Via]		=	preferences->visibilityVias->isChecked();
	CSpecctraObject::globalVisibility()[CSpecctraObject::Pad]		=	preferences->visibilityPads->isChecked();
	CSpecctraObject::globalVisibility()[CSpecctraObject::Plane]		=	preferences->visibilityPlanes->isChecked();
	CSpecctraObject::globalVisibility()[CSpecctraObject::Outline]	=	preferences->visibilityOutlines->isChecked();
	CSpecctraObject::globalVisibility()[CSpecctraObject::Border]	=	preferences->visibilityBorders->isChecked();
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
	CSpecctraObject::globalScene()->update();
}

/**
  * @brief A layer was clicked.
  */
void QAutoRouter::layerClicked(QModelIndex /* idx */)
{
	QListWidgetItem* item = preferences->layerList->currentItem();
	if ( item != NULL && pcb()!=NULL && pcb()->structure()!=NULL )
	{
		QString layerName = item->data(Qt::UserRole).toString();
		CPcbLayer* layer = pcb()->structure()->layer(layerName);
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

