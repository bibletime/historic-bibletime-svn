/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef TOOL_H
#define TOOL_H

#include <QIcon>
#include <QString>
#include <QTextCodec>


class CSwordModuleInfo;
class QLabel;
class QWidget;

namespace util {
namespace tool {

	/**
	 * Converts HTML text to plain text.
	 * This function converts some HTML tags in text (e.g. <br/> to \n)
	 * @return The text withput HTML tags and with converted <br/> to \n
	 */
	QString htmlToText(const QString&);
	/**
	* Converts text to HTML converting some text commands into HTML tags (e.g. \n to <br/>)
	* @return The HTML formatted text we got after changing \n to <br/>
	*/
	QString textToHTML(const QString&);
	/**
	* Creates the file filename and put the text of parameter "text" into the file.
	* @return True if saving was sucessful, otherwise false
	*/
	bool savePlainFile( const QString& filename, const QString& text, const bool& forceOverwrite = false, QTextCodec* fileCodec = QTextCodec::codecForLocale());
	/**
	* Returns the icon used for the module given as aparameter.
	*/
	QIcon getIconForModule( CSwordModuleInfo* );
	/**
	* Returns the name for the icon used for the module given as aparameter.
	*/
	QString getIconNameForModule( CSwordModuleInfo* );

	/** Returns a label to explain difficult things of dialogs.
	* This function returns a label with heading "heading" and explanation "text". This label should be used to
	* explain difficult things of the GUI, e.g. in the optionsdialog.
	*/
	QLabel* explanationLabel(QWidget* parent, const QString& heading, const QString& text );
	/**
	* Returns true if the character at position "pos" of text is inside an HTML tag. Returns false if it's not inside an HTML tag.
	*/
	bool inHTMLTag(int pos, QString & text);

	/** Return the module's tooltip text
	* @param module The module required for the tooltip
	* @return The tooltip text for the passed module
	*/
	QString moduleToolTip(CSwordModuleInfo* module);

	/** Return the module's tooltip text for a remote module
	* @param module The module required for the tooltip
	* @return The tooltip text for the passed module
	*/
	QString remoteModuleToolTip(CSwordModuleInfo* module, QString localVer);

	/**
	* Returns the width in pixels for a string which has mCount 'M' letters, using the specified widget's font.
	* This can be used when setting the size for a widget. It may be better to roughly calculate the size based on some text width rather than use pixels directly.
	*/
	int mWidth(const QWidget* widget, int mCount);
}
}

#endif
