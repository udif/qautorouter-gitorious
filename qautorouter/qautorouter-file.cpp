/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "qautorouter-common.h"

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


