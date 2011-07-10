/*******************************************************************************
* Copyright (C) Pike Aerospace Research Corporation                            *
* Author: Mike Sharkey <mike@pikeaero.com>                                     *
*******************************************************************************/
#include "cspecctrareader.h"

#include "cpcb.h"
#include "cpcbstructure.h"
#include "cpcblayer.h"
#include "cpcbboundary.h"
#include "cpcbplacement.h"
#include "cpcbcomponent.h"
#include "cpcbplace.h"
#include "cpcbimage.h"
#include "cpcblibrary.h"
#include "cpcbpin.h"
#include "cpcboutline.h"
#include "cpcbpath.h"
#include "cpcbpadstack.h"
#include "cpcbnetwork.h"
#include "cpcbnet.h"
#include "cpcbpins.h"
#include "cpcbrule.h"
#include "cpcbvia.h"
#include "cpcbclass.h"
#include "cpcbwidth.h"
#include "cpcbclearance.h"
#include "cpcbtype.h"
#include "cpcbrect.h"

#include "qautorouter.h"

#define inherited QObject

CSpecctraReader::CSpecctraReader(QAutoRouter *parent)
: inherited(parent)
, mTextIndex(0)
, mRoot(NULL)
{
	QObject::connect(parent,SIGNAL(fault(QString)),this,SLOT(faultHandler(QString)));
}

CSpecctraReader::CSpecctraReader(QTextStream& stream, QAutoRouter *parent)
: inherited(parent)
, mTextIndex(0)
, mRoot(NULL)
{
	QObject::connect(parent,SIGNAL(fault(QString)),this,SLOT(faultHandler(QString)));
	read(stream);
}

CSpecctraReader::~CSpecctraReader()
{
}

/**
  * @brief Make the appropriate specctra object based on the class name
  * @brief oClass The specctra class name
  * @return a new CSpecctraObject derivitive of teh appropriate class.
  */
CSpecctraObject* CSpecctraReader::make(QString& oClass,CSpecctraObject* parentObject)
{
	CSpecctraObject* obj;
	oClass = oClass.toLower();
	if (oClass == "pcb")									obj = new CPcb();
	else if (oClass == "structure")							obj = new CPcbStructure();
	else if (oClass == "layer")								obj = new CPcbLayer();
	else if (oClass == "boundary")							obj = new CPcbBoundary();
	else if (oClass == "placement")							obj = new CPcbPlacement();
	else if (oClass == "component")							obj = new CPcbComponent();
	else if (oClass == "place")								obj = new CPcbPlace();
	else if (oClass == "image")								obj = new CPcbImage();
	else if (oClass == "library")							obj = new CPcbLibrary();
	else if (oClass == "outline")							obj = new CPcbOutline();
	else if (oClass == "pin")								obj = new CPcbPin();
	else if (oClass == "path" || oClass == "polygon")		{obj = new CPcbPath(); oClass="path";}
	else if (oClass == "padstack")							obj = new CPcbPadstack();
	else if (oClass == "network")							obj = new CPcbNetwork();
	else if (oClass == "net")								obj = new CPcbNet();
	else if (oClass == "pins")								obj = new CPcbPins();
	else if (oClass == "rule")								obj = new CPcbRule();
	else if (oClass == "via")								obj = new CPcbVia();
	else if (oClass == "class")								obj = new CPcbClass();
	else if (oClass == "width")								obj = new CPcbWidth();
	else if (oClass == "clearance" || oClass == "clear")	{obj = new CPcbClearance(); oClass="clearance";}
	else if (oClass == "type")								obj = new CPcbType();
	else if (oClass == "rect")								obj = new CPcbRect();
	else
		obj = new CSpecctraObject();
	obj->setObjectClass(oClass);
	if ( parentObject != NULL ) /* already in an object? */
	{
		parentObject->appendChild(obj);
	}
	return obj;
}

/**
  * @brief Read in a spectra file from a text stream
  * @param The input text stream, should already be open for reading.
  */
CSpecctraObject* CSpecctraReader::read(QTextStream& stream)
{
	/** start fresh */
	mTextIndex=0;
	mTextBuffer.clear();

	/** read the file */
	mTextBuffer = stream.read(MAX_SPECCTRA_FILESIZE);
	/** and parse it... */
	return parse();
}

/**
  * @brief Parse the text buffer into CSpecctraObject tree.
  * @return the root of the tree.
  */
CSpecctraObject* CSpecctraReader::parse()
{
	/** start fresh */
	CSpecctraObject* object=NULL;
	if ( mRoot != NULL )
	{
		delete mRoot;
		mRoot = NULL;
	}
	while(!eof())
	{
		QString tk = token();
		if ( tk == "(" )			/** beginning a new object */
		{
			QString oClass;
			oClass = word();
			if ( oClass.length() )	/* do we have a class name? */
			{
				CSpecctraObject* newObject = make(oClass,object);
				if ( newObject == NULL )
				{
					emit fault("failed to load.");
					return mRoot;
				}
				object = newObject;
			}
			else
				emit fault("Missing class name.");
		}
		else
		if ( tk == ")" )			/** closing an object */
		{
			if ( object->parentObject() != NULL )
				object = object->parentObject();
		}
		else						/** object attributes */
		{
			object->appendProperty(tk);
		}
	}
	if ( object!=NULL && object->parentObject() != NULL )
		emit fault("Braces mismatch detected.");
	return (mRoot=object);
}

/**
  * @brief Parse the next token from the text buffer.
  * @return As a string, of empty string at end of buffer.
  */
QString CSpecctraReader::token()
{
	QString tk;
	QChar ch = span();
	if ( !ch.isNull() )
	{
		if ( ch == QChar('(') || ch == QChar(')') )		/** beginning and endig of an object */
		{
			tk = next();
		}
		else											/** everything else... */
		{
			tk = word();
		}
	}
	return tk;
}

/**
  * @brief Span white characters.
  * @return peek() char.
  */
QChar CSpecctraReader::span()
{
	while (!eof() && white(peek()))
	{
		next();
	}
	return peek();
}

/**
  * @brief Peek ahead to next character.
  * @return The char or empty char if end of file.
  */
QChar CSpecctraReader::peek()
{
	QChar ch;
	if (!eof())
	{
		ch = mTextBuffer[mTextIndex];
	}
	return ch;
}

/**
  * @brief Return the next character and increment buffer pointer.
  * @return char
  */
QChar CSpecctraReader::next()
{
	QChar ch;
	if (!eof())
	{
		ch = mTextBuffer[mTextIndex++];
	}
	return ch;
}

/**
  * @brief Return the previous character
  */
QChar CSpecctraReader::prev()
{
	if (mTextIndex!=0)
	{
		--mTextIndex;
	}
	return mTextBuffer[mTextIndex];
}

/**
  * @brief Determine the whiteness of a char
  */
bool CSpecctraReader::white(QChar ch)
{
	return (ch==QChar(' ') || ch==QChar('\t') || ch==QChar('\n') || ch==QChar('\r'));
}

/**
  * @brief Parse a word of text
  * @return a word
  */
QString CSpecctraReader::word()
{
	QString w;
	span();
	while(!eof() && !white(peek()) && peek() != QChar('(') && peek() != QChar(')'))
	{
		w += next();
	}
	return w;
}

/**
  * @brief Determine the beginning of file.
  */
bool CSpecctraReader::bof()
{
	return (mTextIndex==0);
}

/**
  * @brief Determine the end of file.
  */
bool CSpecctraReader::eof()
{
	return (mTextIndex>=(unsigned)mTextBuffer.length());
}





