/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#ifndef CSPECCTRAREADER_H
#define CSPECCTRAREADER_H

#include "cspecctraobject.h"

#include <QObject>
#include <QTextStream>

#define MAX_SPECCTRA_FILESIZE 5000000

class QAutoRouter;
class CSpecctraReader : public QObject
{
	Q_OBJECT
	public:
		CSpecctraReader(QAutoRouter *parent);
		CSpecctraReader(QTextStream& stream,QAutoRouter *parent);
		virtual ~CSpecctraReader();

		CSpecctraObject*			read(QTextStream& stream);
		CSpecctraObject*			read();
		CSpecctraObject*			root()	{return mRoot;}

	signals:
		void						fault(QString txt);

	protected:
		CSpecctraObject*			parse();
		QString						token();
		QChar						peek();
		QChar						next();
		QChar						prev();
		QChar						span();
		QString						word();
		bool						white(QChar ch);
		bool						bof();
		bool						eof();
		CSpecctraObject*			make(QString& oClass,CSpecctraObject* parentObject);

		QString						mTextBuffer;
		unsigned int				mTextIndex;
		CSpecctraObject*			mRoot;
};

#endif // CSPECTRAREADER_H

