/***************************************************************************
 *   Copyright (C) 2006 by Martin Gruner   *
 *   mg.pub@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

/* Small speed test program for the versification schema database of BibleTime2.*/

//QT includes
#include <QString>
#include <QStringList>
#include <QtDebug>
#include <QDir>
#include <QFile>
#include <QXmlInputSource>
#include <QXmlReader>
#include <QXmlSimpleReader>
#include <QTime>
#include <QXmlDefaultHandler>

class myHandler: public QXmlDefaultHandler{
public:
	int bibletime_id;
	virtual bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts ){
		QString result;
		result.append("<").append(localName);
		unsigned int count = atts.count();
		for (unsigned int i = 0;i<count;++i){
			result.append(" ").append(atts.localName(i)).append("=\"").append(atts.value(i)).append("\"");
		}
		result.append(" bibletime_id=\"").append(QString::number(++bibletime_id)).append("\">");
		//qDebug() << result;
		return true;
	}
	virtual bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName){
		QString result;
		result.append("</").append(localName).append(">");
		//qDebug() << result;
		return true;
	}

	bool characters ( const QString & ch ){
		//qDebug() << ch;
		return true;
	} 
};


int main(int argc, char *argv[])
{

	QFile xmlfile("test.xml");
	QXmlInputSource xmlsource(&xmlfile);

	QXmlSimpleReader xmlreader;
	myHandler handler;
	handler.bibletime_id = 0;
	xmlreader.setContentHandler(&handler);

	QTime clock;
	clock.start();

	xmlreader.parse(&xmlsource, true);

	qDebug() << "Finished parsing in " << clock.elapsed() << " ms." ;

	
	return EXIT_SUCCESS;
}
