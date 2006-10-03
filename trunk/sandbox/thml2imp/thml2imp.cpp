/***************************************************************************
 *   Copyright (C) 2006 by Martin Gruner   *
 *   mg.pub@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

//C++ includes
#include <iostream>

//QT includes
#include <QString>
#include <QStack>
#include <QStringList>
#include <QtDebug>
#include <QDir>
#include <QFile>
#include <QXmlInputSource>
#include <QXmlReader>
#include <QXmlSimpleReader>
#include <QTime>
#include <QXmlDefaultHandler>

class thmlSplitter: public QXmlDefaultHandler{
public:
	bool inContent;
	QString content;
	QStack<QString> TitleStack;

	QString shortTitle(const QString longTitle)
	{
		QString result;
		QStringList words = longTitle.split(" ", QString::SkipEmptyParts);
		const unsigned int maxWords = 7;
		const unsigned int wordCount = words.count();
		for (int i = 0; i < wordCount && i < maxWords; i++)
		{
			result.append(words[i]);
			if (i < (wordCount-1)) result.append(" ");
		}
		if (wordCount > maxWords) result.append("...");
		return result;
	}

	virtual bool startElement ( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts ){
		if (localName.startsWith("div1") || localName.startsWith("div2") || localName.startsWith("div3") || localName.startsWith("div4")) 
		{
			inContent = true;
			TitleStack.push(shortTitle(atts.value("title")));

			QString output;
			output.append("$$$");
			QVectorIterator<QString> i(TitleStack);
			while (i.hasNext())
				output.append("/").append(i.next());
			std::cout << "\n" << output.toStdString() << "\n"; // << content;
		}
		else if (inContent) //Tag ausgeben
		{
			content.append("<").append(localName);
			unsigned int count = atts.count();
			for (unsigned int i = 0;i<count;++i){
				content.append(" ").append(atts.localName(i)).append("=\"").append(atts.value(i)).append("\"");
			}
			content.append(">");
		}
		return true;
	}

	virtual bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName){
		if (localName.startsWith("div1") || localName.startsWith("div2") || localName.startsWith("div3") || localName.startsWith("div4")) 
		{
			std::cout << content.toStdString();
			content.clear();
			TitleStack.pop();
		}
		else //Tag ausgeben
		{
			if (inContent) content.append("</").append(localName).append(">");
		}
		return true;
	}

	bool characters ( const QString & ch ){
		if (inContent) content.append(ch);
		return true;
	} 
};


int main(int argc, char *argv[])
{
	QFile xmlfile( argv[1] );
	QXmlInputSource xmlsource(&xmlfile);

	QXmlSimpleReader xmlreader;
	thmlSplitter handler;
	handler.inContent = false;
	xmlreader.setContentHandler(&handler);

	xmlreader.parse(&xmlsource, true);

	return EXIT_SUCCESS;
}
