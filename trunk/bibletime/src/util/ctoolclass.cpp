/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2007 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "ctoolclass.h"

#include "util/cresmgr.h"
#include "util/directoryutil.h"
#include "backend/managers/cswordbackend.h"
#include "backend/drivers/cswordmoduleinfo.h"

//Qt
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QTextCodec>
#include <QRegExp>

//KDE includes
#include <klocale.h>
#include <kstandarddirs.h>
#include <kmessagebox.h>

QString CToolClass::locatehtml(const QString &filename) {
	QString path = KStandardDirs::locate("html", KGlobal::locale()->language() + '/' + filename);
	if (path.isNull())
		path = KStandardDirs::locate("html", "default/" + filename);
	if (path.isNull())
		path = KStandardDirs::locate("html", "en/" + filename);
	return path;
}

/** Converts HTML text to plain text */
QString CToolClass::htmlToText(const QString& html) {
	QString newText = html;
	// convert some tags we need in code
	newText.replace( QRegExp(" "),"#SPACE#" );
	newText.replace( QRegExp("<br/?>\\s*"), "<br/>\n" );
	newText.replace( QRegExp("#SPACE#")," " );

	QRegExp re("<.+>");
	re.setMinimal(true);
	newText.replace( re,"" );
	return newText;
}

/** Converts text to HTML (\n to <BR>) */
QString CToolClass::textToHTML(const QString& text) {
	QString newText = text;
	newText.replace( QRegExp("<BR>\n"),"#NEWLINE#" );
	newText.replace( QRegExp("\n"),"<BR>\n" );
	newText.replace( QRegExp("#NEWLINE#"),"<BR>\n");
	return newText;
}

/** Creates the file filename and put text into the file.
 */
bool CToolClass::savePlainFile( const QString& filename, const QString& text, const bool& forceOverwrite, QTextCodec* fileCodec) {
	QFile saveFile(filename);
	bool ret;

	if (saveFile.exists()) {
		if (!forceOverwrite && KMessageBox::warningYesNo(0,
				QString::fromLatin1("<qt><B>%1</B><BR>%2</qt>")
				.arg( i18n("The file already exists.") )
				.arg( i18n("Do you want to overwrite it?")
					)
														) == KMessageBox::No
		   ) {
			return false;
		}
		else { //either the user chose yes or forceOverwrite is set
			saveFile.remove();
		}
	};

	if ( saveFile.open(QIODevice::ReadWrite) ) {
		QTextStream textstream( &saveFile );
		textstream.setCodec(fileCodec);
		textstream << text;
		saveFile.close();
		ret = true;
	}
	else {
		KMessageBox::error(0, QString::fromLatin1("<qt>%1<BR><B>%2</B></qt>")
						   .arg( i18n("The file couldn't be saved.") )
						   .arg( i18n("Please check permissions etc.")));
		saveFile.close();
		ret = false;
	}
	return ret;
}


/** Returns the icon used for the module given as aparameter. */
QIcon CToolClass::getIconForModule( CSwordModuleInfo* module_info ) {
	if (!module_info) return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::book::icon_locked);

	if (module_info->category() == CSwordModuleInfo::Cult) {
		return util::filesystem::DirectoryUtil::getIcon("stop.svg");
	}

	switch (module_info->type()) {
		case CSwordModuleInfo::Bible:
			if (module_info->isLocked())
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::bible::icon_locked);
			else
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::bible::icon_unlocked);
			break;

		case CSwordModuleInfo::Lexicon:
			if (module_info->isLocked())
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::lexicon::icon_locked);
			else
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::lexicon::icon_unlocked);
			break;

		case CSwordModuleInfo::Commentary:
			if (module_info->isLocked())
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::commentary::icon_locked);
			else
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::commentary::icon_unlocked);
			break;

		case CSwordModuleInfo::GenericBook:
			if (module_info->isLocked())
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::book::icon_locked);
			else
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::book::icon_unlocked);
			break;

		case CSwordModuleInfo::Unknown: //fallback
			default:
			if (module_info->isLocked())
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::book::icon_locked);
			else
				return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::book::icon_unlocked);
			break;
	}
	return util::filesystem::DirectoryUtil::getIcon(CResMgr::modules::book::icon_unlocked);
}

QLabel* CToolClass::explanationLabel(QWidget* parent, const QString& heading, const QString& text ) {
	QLabel* label = new QLabel( QString::fromLatin1("<B>%1</B><BR>%2").arg(heading).arg(text),parent );
//	label->setAutoResize(true);
	label->setMargin(1);
	label->setFrameStyle(QFrame::Box | QFrame::Plain);
	return label;
}

/** No descriptions */
bool CToolClass::inHTMLTag(int pos, QString & text) {
	int i1=text.lastIndexOf("<",pos);
	int i2=text.lastIndexOf(">",pos);
	int i3=text.indexOf(">",pos);
	int i4=text.indexOf("<",pos);


	// if ((i1>0) && (i2==-1))  //we're in th first html tag
	//  i2=i1; // not ncessary, just for explanation

	if ((i3>0) && (i4==-1))  //we're in the last html tag
		i4=i3+1;

	//  qWarning("%d > %d && %d < %d",i1,i2,i3,i4);

	if ( (i1>i2) && (i3<i4) )
		return true; //yes, we're in a tag

	return false;
}

QString CToolClass::moduleToolTip(CSwordModuleInfo* module) {
	Q_ASSERT(module);
	if (!module) {
		return QString::null;
	}

	QString text;

	text = QString("<b>%1</b> ").arg( module->name() )
		   + ((module->category() == CSwordModuleInfo::Cult) ? QString::fromLatin1("<small><b>%1</b></small><br>").arg(i18n("Take care, this work contains cult / questionable material!")) : QString::null);

	text += QString("<small>(") + module->config(CSwordModuleInfo::Description) + QString(")</small><hr>");

	text += i18n("Language") + QString(": %1<br>").arg( module->language()->translatedName() );

	if (module->isEncrypted()) {
		text += i18n("Unlock key") + QString(": %1<br>")
				.arg(!module->config(CSwordModuleInfo::CipherKey).isEmpty() ? module->config(CSwordModuleInfo::CipherKey) : QString("<font COLOR=\"red\">%1</font>").arg(i18n("not set")));
	}

	if (module->hasVersion()) {
		text += i18n("Version") + QString(": %1<br>").arg( module->config(CSwordModuleInfo::ModuleVersion) );
	}

	QString options;
	unsigned int opts;
	for (opts = CSwordModuleInfo::filterTypesMIN; opts <= CSwordModuleInfo::filterTypesMAX; ++opts) {
		if (module->has( static_cast<CSwordModuleInfo::FilterTypes>(opts) )) {
			if (!options.isEmpty()) {
				options += QString::fromLatin1(", ");
			}

			options += CSwordBackend::translatedOptionName(
						   static_cast<CSwordModuleInfo::FilterTypes>(opts)
					   );
		}
	}

	if (!options.isEmpty()) {
		text += i18n("Options") + QString::fromLatin1(": <small>") + options + QString("</small>");
	}

	if (text.right(4) == QString::fromLatin1("<br>")) {
		text = text.left(text.length()-4);
	}

	return text;
}
