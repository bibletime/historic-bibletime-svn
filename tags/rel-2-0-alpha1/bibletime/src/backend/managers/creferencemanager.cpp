/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "creferencemanager.h"
#include "backend/keys/cswordversekey.h"

#include "backend/config/cbtconfig.h"
#include "util/cpointers.h"

//QT
#include <QRegExp>

//stl
#include <algorithm>       // STL algorithms class library

/** Returns a hyperlink used to be imbedded in the display windows. At the moment the format is sword://module/key */
const QString CReferenceManager::encodeHyperlink( const QString moduleName, const QString key, const CReferenceManager::Type type) {
	QString ret = QString::null;

	switch (type) {

		case Bible:
			ret = QString("sword://Bible/");
			break;
		case Commentary:
			ret = QString("sword://Commentary/");
			break;
		case Lexicon:
			ret = QString("sword://Lexicon/");
			break;
		case GenericBook:
			ret = QString("sword://Book/");
			break;
		case MorphHebrew:
			ret = QString("morph://Hebrew/");
			break;
		case MorphGreek:
			ret = QString("morph://Greek/");
			break;
		case StrongsHebrew:
			ret = QString("strongs://Hebrew/");
			break;
		case StrongsGreek:
			ret = QString("strongs://Greek/");
			break;
		default:
			break;
	}

	if (!moduleName.isEmpty()) {
		ret.append( moduleName ).append('/');
	}
	else { //if module is empty use fallback module
		ret.append( preferredModule(type) ).append('/');
	}

	if (type == GenericBook) {
		const QString s = (!key.isEmpty() ? key : QString::null);
		QString newKey = QString::null;
		//replace all / of the key (e.g. of a CSwordTreeKey) with
		// the escape sequence \/ so we know it's a link internal divider (e.g. of CSwordTreeKey)!

		QChar c;

		for(int i = 0; i < s.length(); ++i) {
			c = s.at(i);

			if (c == '/') {
				newKey.append("\\/");
			}
			else {
				newKey.append(c);
			}
		}

		ret.append( newKey );
	}
	else { //slashes do not appear in verses and dictionary entries

		switch (type) {

			case Bible: //bibles or commentary keys need parsing

			case Commentary: {
/*  				CSwordModuleInfo* mod = CPointers::backend()->findModuleByName(moduleName);

  				ParseOptions options;
  				options.refDestinationModule = mod->name();
  				options.refBase =
  				options.sourceLanguage = mod->module()->Lang();
  				options.destinationLanguage = "en";

				ret.append( parseVerseReference(key, options) ); //we add the english key, so drag and drop will work in all cases*/
				ret.append(key);
				break;
			}

			default:
				ret.append( key ); //use the standard key, no parsing required
				break;
		}
	}

	return ret;
}

/** Decodes the given hyperlink to module and key. */
bool CReferenceManager::decodeHyperlink( const QString& hyperlink, QString& module, QString& key, CReferenceManager::Type& type ) {
	/**
	* We have to decide between three types of URLS: sword://Type/Module/Key, morph://Testament/key and strongs://Testament/Key
	*/
	module = QString::null;
	key = QString::null;

	type = Unknown; //not yet known
	QString ref = hyperlink;
	//remove the trailing slash

	if (ref.right(1)=="/" && ref.right(2) != "\\/") //trailing slash, but not escaped
		ref = ref.left(ref.length()-1);

	//find out which type we have by looking at the beginning (protocoll section of URL)
	if (ref.left(8).toLower() == "sword://") { //Bible, Commentary or Lexicon
		ref = ref.mid(8);

		if (ref.left(5).toLower() == "bible") { //a bible hyperlink
			type = CReferenceManager::Bible;
			ref = ref.mid(6); //inclusive trailing slash
		}
		else if (ref.left(10).toLower() == "commentary") { // a Commentary hyperlink
			type = CReferenceManager::Commentary;
			ref = ref.mid(11); //inclusive trailing slash
		}
		else if (ref.left(7).toLower() == "lexicon") { // a Lexicon hyperlink
			type = CReferenceManager::Lexicon;
			ref = ref.mid(8); //inclusive trailing slash
		}
		else if (ref.left(4).toLower() == "book") { // a Book hyperlink
			type = CReferenceManager::GenericBook;
			ref = ref.mid(5); //inclusive trailing slash
		}

		// string up to next slash is the modulename
		if (ref.at(0) != '/' ) { //we have a module given

			while (true) {
				const int pos = ref.indexOf("/");

				if ((pos>0) && ref.at(pos-1) != '\\') { //found a slash which is not escaped
					module = ref.mid(0,pos);
					ref = ref.mid(pos+1);
					break;
				}
				else if (pos == -1) {
					break;
				}
			}

			// the rest is the key
			key = ref;
		}
		else {
			key = ref.mid(1);
		}

		//the key may be an osis key like "NASBLex:Moses", which sets the module, too
		//   const int modPos = key.find(":");
		//   if (modPos != -1 && key.at(modPos-1).isLetter() && key.at(modPos+1).isLetter()) {
		//    module = key.left(modPos);
		//    key = key.mid(modPos+1);
		//
		//    qWarning("found the module name %s with key %s", module.latin1(), key.latin1());
		//   }

		//replace \/ escapes with /
		key.replace(QRegExp("\\\\/"), "/");
	}
	else if (ref.left(8).toLower() == "morph://" || ref.left(10).toLower() == "strongs://") { //strongs or morph URL have the same format
		enum PreType {IsMorph, IsStrongs};
		PreType preType = IsMorph;

		if (ref.left(8).toLower() == "morph://") { //morph code hyperlink
			ref = ref.mid(8);
			preType = IsMorph;
		}
		else if (ref.left(10).toLower() == "strongs://") {
			ref = ref.mid(10);
			preType = IsStrongs;
		}

		//part up to next slash is the language
		const int pos = ref.indexOf("/");

		if (pos>0) { //found
			const QString language = ref.mid(0,pos);

			if (language.toLower() == "hebrew") {
				switch (preType) {

					case IsMorph:
					type = CReferenceManager::MorphHebrew;
					break;

					case IsStrongs:
					type = CReferenceManager::StrongsHebrew;
					break;
				}
			}
			else if (language.toLower() == "greek") {
				switch (preType) {

					case IsMorph:
					type = CReferenceManager::MorphGreek;
					break;

					case IsStrongs:
					type = CReferenceManager::StrongsGreek;
					break;
				}
			}

			ref = ref.mid(pos+1);
			key = ref; //the remaining part is the key

			module = preferredModule(type);
		}
	}

	if (key.isEmpty() && module.isEmpty())
		return false;

	return true;
}

const QString CReferenceManager::encodeReference(const QString &module, const QString &reference) {
	//return QString("(%1)%2").arg(module).arg(reference);
	return QString("(").append(module).append(")").append(reference);
}

void CReferenceManager::decodeReference(QString &dragreference, QString &module, QString &reference) {
	const int pos = dragreference.indexOf(")");
	const QString fallbackModule = dragreference.mid( 1, pos - 1);
	dragreference = dragreference.mid(pos+1);

	module = fallbackModule;
	reference = dragreference;
}

/** Returns true if the parameter is a hyperlink. */
bool CReferenceManager::isHyperlink( const QString& hyperlink ) {
	return (    hyperlink.left(8)  == "sword://")
		   || (hyperlink.left(10) == "strongs://")
		   || (hyperlink.left(8)  == "morph://");
}

/** Returns the preferred module name for the given type. */
const QString CReferenceManager::preferredModule( const CReferenceManager::Type type ) {
	QString moduleName = QString::null;
	CSwordModuleInfo* module = 0;

	switch (type) {

		case CReferenceManager::Bible:

		module = CBTConfig::get
					 ( CBTConfig::standardBible );

		break;

		case CReferenceManager::Commentary:
		module = CBTConfig::get
					 ( CBTConfig::standardCommentary );

		break;

		case CReferenceManager::Lexicon:
		module = CBTConfig::get
					 ( CBTConfig::standardLexicon );

		break;

		case CReferenceManager::StrongsHebrew:
		module = CBTConfig::get
					 ( CBTConfig::standardHebrewStrongsLexicon );

		break;

		case CReferenceManager::StrongsGreek:
		module = CBTConfig::get
					 ( CBTConfig::standardGreekStrongsLexicon );

		break;

		case CReferenceManager::MorphHebrew:
		module = CBTConfig::get
					 ( CBTConfig::standardHebrewMorphLexicon );

		break;

		case CReferenceManager::MorphGreek:
		module = CBTConfig::get
					 ( CBTConfig::standardGreekMorphLexicon );

		break;

		default:
		module = 0;

		break;
	}

	return module ? module->name() : QString::null;
}

/** No descriptions */
CReferenceManager::Type CReferenceManager::typeFromModule( const CSwordModuleInfo::ModuleType type) {
	switch (type) {

		case CSwordModuleInfo::Bible:
		return CReferenceManager::Bible;

		case CSwordModuleInfo::Commentary:
		return CReferenceManager::Commentary;

		case CSwordModuleInfo::Lexicon:
		return CReferenceManager::Lexicon;

		case CSwordModuleInfo::GenericBook:
		return CReferenceManager::GenericBook;

		default:
		return CReferenceManager::Unknown;
	}
}

/** Parses the given verse references using the given language and the module.*/
const QString CReferenceManager::parseVerseReference( const QString& ref, const CReferenceManager::ParseOptions& options) {

	CSwordModuleInfo* const mod = CPointers::backend()->findModuleByName(options.refDestinationModule);
	Q_ASSERT(mod);

	if (!mod) {
		//parsing of non-verse based references is not supported
		return ref;
	}

	if ((mod->type() != CSwordModuleInfo::Bible) && (mod->type() != CSwordModuleInfo::Commentary)) {
		qDebug("CReferenceManager: Only verse based modules are supported as ref destination module");
		return QString::null;
	}

	QString sourceLanguage = options.sourceLanguage;
	QString destinationLanguage = options.destinationLanguage;

 	sword::StringList locales = sword::LocaleMgr::getSystemLocaleMgr()->getAvailableLocales();
 	if (/*options.sourceLanguage == "en" ||*/ std::find(locales.begin(), locales.end(), sourceLanguage.toUtf8().constData()) == locales.end()) { //sourceLanguage not available
		sourceLanguage = "en_US";
 	}

 	if (/*options.destinationLanguage == "en" ||*/ std::find(locales.begin(), locales.end(), sourceLanguage.toUtf8().constData()) == locales.end()) { //destination not available
		destinationLanguage = "en_US";
 	}

	QString ret;
	QStringList refList = ref.split(";");

	CSwordVerseKey baseKey(0);
	baseKey.setLocale( sourceLanguage.toUtf8().constData() );
	baseKey.key( options.refBase ); //probably in the sourceLanguage
 	baseKey.setLocale( "en_US" ); //english works in all environments as base

// 	CSwordVerseKey dummy(0);
	//HACK: We have to workaround a Sword bug, we have to set the default locale to the same as the sourceLanguage !
	const QString oldLocaleName = CPointers::backend()->booknameLanguage();
	CPointers::backend()->booknameLanguage(sourceLanguage);

	sword::VerseKey dummy;
 	dummy.setLocale( sourceLanguage.toUtf8().constData() );
 	Q_ASSERT( !strcmp(dummy.getLocale(), sourceLanguage.toUtf8().constData()) );

// 	qDebug("Parsing '%s' in '%s' using '%s' as base, source lang '%s', dest lang '%s'", ref.latin1(), options.refDestinationModule.latin1(), baseKey.key().latin1(), sourceLanguage.latin1(), destinationLanguage.latin1());

	for (QStringList::iterator it = refList.begin(); it != refList.end(); it++) {
		//The listkey may contain more than one item, because a ref lik "Gen 1:3,5" is parsed into two single refs
		sword::ListKey lk = dummy.ParseVerseList((*it).toUtf8().constData(), baseKey.key().toUtf8().constData(), true);
		Q_ASSERT(!dummy.Error());

		//Q_ASSERT(lk.Count());
		if (!lk.Count()) {
			ret.append( *it ); //don't change the original
			continue;
		}

		for (int i = 0; i < lk.Count(); ++i) {
			if (dynamic_cast<sword::VerseKey*>(lk.getElement(i))) { // a range
				sword::VerseKey* k = dynamic_cast<sword::VerseKey*>(lk.getElement(i));
				Q_ASSERT(k);
 				k->setLocale( destinationLanguage.toUtf8().constData() );

				ret.append( QString::fromUtf8(k->getRangeText()) ).append("; ");
			}
			else { // a single ref
				sword::VerseKey vk;
 				vk.setLocale( sourceLanguage.toUtf8().constData() );
				vk = lk.getElement(i)->getText();
				vk.setLocale( destinationLanguage.toUtf8().constData() );

				ret.append( QString::fromUtf8(vk.getText()) ).append("; ");
			}
		}

	}

	CPointers::backend()->booknameLanguage(oldLocaleName);
	return ret;
}
