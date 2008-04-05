/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "clanguagemgr.h"

#include "backend/drivers/cswordmoduleinfo.h"
#include "cswordbackend.h"

#include "util/cpointers.h"

//KDE


CLanguageMgr::Language::Language() {}

CLanguageMgr::Language::Language(const Language& l) {
	m_abbrev = l.m_abbrev;
	m_englishName = l.m_englishName;
	m_translatedName = l.m_translatedName;
	m_altAbbrevs = l.m_altAbbrevs;
}

CLanguageMgr::Language::Language( const QString& abbrev, const QString& name, const QString& translatedName, const QStringList& altAbbrevs ) {
	m_abbrev = abbrev;
	m_englishName = name;
	m_translatedName = translatedName;
	m_altAbbrevs =  altAbbrevs;
}

CLanguageMgr::Language::~Language() {
}


/****************************************************/
/******************** CLanguageMgr ******************/
/****************************************************/
CLanguageMgr::CLanguageMgr() : m_langMap() {
	m_availableModulesCache.moduleCount = 0;
	init();
}

CLanguageMgr::~CLanguageMgr() {
	qDeleteAll(m_cleanupLangPtrs);
	m_cleanupLangPtrs.clear();
	qDeleteAll(m_langList);
	m_langList.clear();
}

const CLanguageMgr::LangMap& CLanguageMgr::availableLanguages() {
	QList<CSwordModuleInfo*> mods = CPointers::backend()->moduleList();

	if ( m_availableModulesCache.moduleCount != (unsigned int)mods.count() ) { //we have to refill the cached map
		m_availableModulesCache.availableLanguages.clear();
		m_availableModulesCache.moduleCount = mods.count();

		//collect the languages abbrevs of all modules
		QStringList abbrevs;

		foreach (const CSwordModuleInfo* mod,  mods) {
			if (!abbrevs.contains(mod->module()->Lang())){
				abbrevs.append(mod->module()->Lang());
			}
		}

		//now create a map of available langs
		foreach ( QString abbrev, abbrevs ) {
			const Language* const lang = languageForAbbrev(abbrev);

			if (lang->isValid()) {
				m_availableModulesCache.availableLanguages.insert( abbrev, lang );
			}
			else { //invalid lang used by a module, create a new language using the abbrev
				Language* newLang = new Language(abbrev, abbrev, abbrev);
				m_cleanupLangPtrs.append(newLang);
				m_availableModulesCache.availableLanguages.insert( abbrev, newLang );
			}
		}
	}
	return m_availableModulesCache.availableLanguages;
}

const CLanguageMgr::Language* const CLanguageMgr::languageForAbbrev( const QString& abbrev ) const {
	LangMapIterator it = m_langMap.find(abbrev);
	if (it != m_langMap.constEnd()) return *it; //Language is already here
	
	//try to search in the alternative abbrevs
	foreach (const Language* lang, m_langList ) {
		if (lang->alternativeAbbrevs().contains(abbrev)) return lang;
	}
	
	// Invalid lang used by a modules, create a new language using the abbrev
	Language* newLang = new Language(abbrev, abbrev, abbrev); //return a language which holds the valid abbrev
	m_cleanupLangPtrs.append(newLang);

	return newLang;
}

const CLanguageMgr::Language* const CLanguageMgr::languageForName( const QString& name ) const {
	foreach ( const Language* lang, m_langList ) {
		if (lang->name() == name) return lang;
	}
	return &m_defaultLanguage;//invalid language
}

const CLanguageMgr::Language* const CLanguageMgr::languageForTranslatedName( const QString& name ) const {
	foreach ( const Language* lang, m_langList ) {
		if (lang->translatedName() == name) return lang;
	}
	return &m_defaultLanguage; //invalid language
}

void CLanguageMgr::init() {
	//if we've already inserted all items we do not proceed
	if (m_langMap.count() > 0) return;

	/*
	* Chris explained in an eMail how language codes are build:
	 
	Preference order for locale codes are:
	 
	ISO 639-1
	ISO 639-2
	Ethnologue (http://www.ethnologue.org/)
	 
	We intend to always follow OSIS locale conventions, which state that
	Ethnologue codes will be encoded in the format x-E-??? where ???
	represents the 3-letter Ethnologue code in capital letters (though
	capitalization really doesn't matter here).
	 
	Some older modules maintain a former format for Ethnolgoue codes of
	xx-???, including the AleWiesler module.
	 
	--Chris
	 
	*/

	//  m_langList.append( new Language("aa"  , "Afar"        , QObject::tr("Afar")) );
	//  m_langList.append( new Language("ab"  , "Abkhazian"   , QObject::tr("Abkhazian")) );
	//  m_langList.append( new Language("ae"  , "Avestan"     , QObject::tr("Avestan")) );
	m_langList.append( new Language("af"  , "Afrikaans"   , QObject::tr("Afrikaans")) );
	//  m_langList.append( new Language("am"  , "Amharic"     , QObject::tr("Amharic")) );
	m_langList.append( new Language("ang", "English, Old (ca.450-1100)", QObject::tr("English, Old (ca.450-1100)")) );
	m_langList.append( new Language("ar"  , "Arabic"      , QObject::tr("Arabic")) );
	//  m_langList.append( new Language("as"  , "Assamese"    , QObject::tr("Assamese")) );
	m_langList.append( new Language("az"  , "Azerbaijani" , QObject::tr("Azerbaijani")) );
	//  m_langList.append( new Language("ba"  , "Bashkir"     , QObject::tr("Bashkir")) );
	m_langList.append( new Language("bar"  , "Bavarian"    , QObject::tr("Bavarian")) );
	m_langList.append( new Language("be"  , "Belarusian"  , QObject::tr("Belarusian")) );
	m_langList.append( new Language("bg"  , "Bulgarian"   , QObject::tr("Bulgarian")) );
	//  m_langList.append( new Language("bh"  , "Bihari"      , QObject::tr("Bihari")) );
	//  m_langList.append( new Language("bi"  , "Bislama"     , QObject::tr("Bislama")) );
	//  m_langList.append( new Language("bn"  , "Bengali"     , QObject::tr("Bengali")) );
	//  m_langList.append( new Language("bo"  , "Tibetan"     , QObject::tr("Tibetan")) );
	m_langList.append( new Language("br"  , "Breton"      , QObject::tr("Breton")) );
	m_langList.append( new Language("bs"  , "Bosnian"     , QObject::tr("Bosnian")) );
	m_langList.append( new Language("ca"  , "Catalan"     , QObject::tr("Catalan")) );
	//  m_langList.append( new Language("ce"  , "Chechen"     , QObject::tr("Chechen")) );
	m_langList.append( new Language("ceb" , "Cebuano"     , QObject::tr("Cebuano")) );
	m_langList.append( new Language("ch"  , "Chamorro"    , QObject::tr("Chamorro")) );
	//  m_langList.append( new Language("co"  , "Corsican"    , QObject::tr("Corsican")) );
	m_langList.append( new Language("cop" , "Coptic"         , QObject::tr("Coptic")) );
	m_langList.append( new Language("cs"  , "Czech"       , QObject::tr("Czech")) );
	m_langList.append( new Language("cu"  , "Church Slavic" , QObject::tr("Church Slavic")) );
	//  m_langList.append( new Language("cv"  , "Chuvash"     , QObject::tr("Chuvash")) );
	m_langList.append( new Language("cy"  , "Welsh"       , QObject::tr("Welsh")) );
	m_langList.append( new Language("da"  , "Danish"      , QObject::tr("Danish")) );
	m_langList.append( new Language("de"  , "German"      , QObject::tr("German")) );
	//  m_langList.append( new Language("dz"  , "Dzongkha"    , QObject::tr("Dzongkha")) );
	m_langList.append( new Language("el"  , "Greek, Modern (1453-)" , QObject::tr("Greek, Modern (1453-)"), makeStringList("gre;ell")) );
	m_langList.append( new Language("en"  , "English"     , QObject::tr("English")) );
	m_langList.append( new Language("en_US","American English"     , QObject::tr("American English")) );
	m_langList.append( new Language("enm" , "English, Middle (1100-1500)", QObject::tr("English, Middle (1100-1500)")) );
	m_langList.append( new Language("eo"  , "Esperanto"   , QObject::tr("Esperanto")) );
	m_langList.append( new Language("es"  , "Spanish"     , QObject::tr("Spanish")) );
	m_langList.append( new Language("et"  , "Estonian"    , QObject::tr("Estonian")) );
	m_langList.append( new Language("eu"  , "Basque"      , QObject::tr("Basque")) );
	m_langList.append( new Language("fa"  , "Persian"     , QObject::tr("Persian")) );
	m_langList.append( new Language("fi"  , "Finnish"     , QObject::tr("Finnish")) );
	//  m_langList.append( new Language("fj"  , "Fijian"      , QObject::tr("Fijian")) );
	//  m_langList.append( new Language("fo"  , "Faroese"     , QObject::tr("Faroese")) );
	m_langList.append( new Language("fr"  , "French"      , QObject::tr("French")) );
	m_langList.append( new Language("fy"  , "Frisian"     , QObject::tr("Frisian")) );
	m_langList.append( new Language("ga"  , "Irish"       , QObject::tr("Irish")) );
	m_langList.append( new Language("gd"  , "Gaelic (Scots)", QObject::tr("Gaelic (Scots)")) );
	//  m_langList.append( new Language("gl"  , "Gallegan"    , QObject::tr("Gallegan")) );
	//  m_langList.append( new Language("gn"  , "Guarani"     , QObject::tr("Guarani")) );
	//  m_langList.append( new Language("gn"  , "Gujarati"    , QObject::tr("Gujarati")) );
	m_langList.append( new Language("got"  , "Gothic"      , QObject::tr("Gothic")) );
	m_langList.append( new Language("gv"   , "Manx"        , QObject::tr("Manx")) );
	m_langList.append( new Language("grc"  , "Greek, Ancient (to 1453)" , QObject::tr("Greek, Ancient (to 1453)")) );
	m_langList.append( new Language("he"  , "Hebrew"      , QObject::tr("Hebrew")) );
	m_langList.append( new Language("hau" , "Hausa"      , QObject::tr("Hausa")) );
	m_langList.append( new Language("haw" , "Hawaiian"    , QObject::tr("Hawaiian")) );
	m_langList.append( new Language("hi"  , "Hindi"       , QObject::tr("Hindi")) );
	//  m_langList.append( new Language("ho"  , "Hiri Motu"   , QObject::tr("Hiri Motu")) );
	m_langList.append( new Language("hr"  , "Croatian"    , QObject::tr("Croatian")) );
	m_langList.append( new Language("ht"  , "Haitian Creole"    , QObject::tr("Haitian Creole")) );
	m_langList.append( new Language("hu"  , "Hungarian"   , QObject::tr("Hungarian")) );
	m_langList.append( new Language("hy"  , "Armenian"    , QObject::tr("Armenian")) );
	//  m_langList.append( new Language("hz"  , "Herero"      , QObject::tr("Herero")) );
	//  m_langList.append( new Language("ia"  , "Interlingua" , QObject::tr("Interlingua")) );
	m_langList.append( new Language("id"  , "Indonesian"  , QObject::tr("Indonesian")) );
	//  m_langList.append( new Language("ie"  , "Interlingue" , QObject::tr("Interlingue")) );
	//  m_langList.append( new Language("ik"  , "Inupiaq"     , QObject::tr("Inupiaq")) );
	m_langList.append( new Language("is"  , "Icelandic"   , QObject::tr("Icelandic")) );
	m_langList.append( new Language("it"  , "Italian"     , QObject::tr("Italian")) );
	//  m_langList.append( new Language("iu"  , "Inuktitut"   , QObject::tr("Inuktitut")) );
	m_langList.append( new Language("ja"  , "Japanese"    , QObject::tr("Japanese")) );
	m_langList.append( new Language("ka"   , "Georgian"  , QObject::tr("Georgian")) );
	//  m_langList.append( new Language("x-E-KAB"   , "Kabyle"    , QObject::tr("Kabyle")) );
	m_langList.append( new Language("kek"  , "Kekchi"    , QObject::tr("Kekchi")) );
	//  m_langList.append( new Language("ki"  , "Kikuyu"      , QObject::tr("Kikuyu")) );
	//  m_langList.append( new Language("kj"  , "Kuanyama"    , QObject::tr("Kuanyama")) );
	//  m_langList.append( new Language("kk"  , "Kazakh"      , QObject::tr("Kazakh")) );
	//  m_langList.append( new Language("kl"  , "Kalaallisut" , QObject::tr("Kalaallisut")) );
	//  m_langList.append( new Language("km"  , "Khmer"       , QObject::tr("Khmer")) );
	//  m_langList.append( new Language("kn"  , "Kannada"     , QObject::tr("Kannada")) );
	m_langList.append( new Language("ko"  , "Korean"      , QObject::tr("Korean")) );
	//  m_langList.append( new Language("ks"  , "Kashmiri"    , QObject::tr("Kashmiri")) );
	m_langList.append( new Language("ku"  , "Kurdish"     , QObject::tr("Kurdish")) );
	//  m_langList.append( new Language("kv"  , "Komi"        , QObject::tr("Komi")) );
	//  m_langList.append( new Language("kw"  , "Cornish"     , QObject::tr("Cornish")) );
	m_langList.append( new Language("ky"  , "Kirghiz"     , QObject::tr("Kirghiz")) );
	m_langList.append( new Language("la"  , "Latin"       , QObject::tr("Latin")) );
	//  m_langList.append( new Language("lb"  , "Letzeburgesch" , QObject::tr("Letzeburgesch")) );
	m_langList.append( new Language("lmo"  , "Lombard"    , QObject::tr("Lombard")) );
	//  m_langList.append( new Language("ln"  , "Lingala"     , QObject::tr("Lingala")) );
	//  m_langList.append( new Language("lo"  , "Lao"         , QObject::tr("Lao")) );
	m_langList.append( new Language("lt"  , "Lithuanian"  , QObject::tr("Lithuanian")) );
	m_langList.append( new Language("lv"  , "Latvian"     , QObject::tr("Latvian")) );
	//  m_langList.append( new Language("mg"  , "Malagasy"    , QObject::tr("Malagasy")) );
	//  m_langList.append( new Language("mh"  , "Marshall"    , QObject::tr("Marshall")) );
	m_langList.append( new Language("mi"  , "Maori"       , QObject::tr("Maori")) );
	m_langList.append( new Language("mk"  , "Macedonian"  , QObject::tr("Macedonian")) );
	//  m_langList.append( new Language("ml"  , "Malayalam"   , QObject::tr("Malayalam")) );
	//  m_langList.append( new Language("mn"  , "Mongolian"   , QObject::tr("Mongolian")) );
	//  m_langList.append( new Language("mo"  , "Moldavian"   , QObject::tr("Moldavian")) );
	m_langList.append( new Language("mos"  , "More"   , QObject::tr("More")) );
	//  m_langList.append( new Language("mr"  , "Marathi"     , QObject::tr("Marathi")) );
	m_langList.append( new Language("ms"  , "Malay"       , QObject::tr("Malay")) );
	m_langList.append( new Language("mt"  , "Maltese"     , QObject::tr("Maltese")) );
	m_langList.append( new Language("my"  , "Burmese"     , QObject::tr("Burmese")) );
	//  m_langList.append( new Language("na"  , "Nauru"       , QObject::tr("Nauru")) );
	//  m_langList.append( new Language("nb"  , "Norwegian Bokmål"       , QObject::tr("Norwegian Bokmål")) );
	//  m_langList.append( new Language("nd"  , "Ndebele, North" , QObject::tr("Ndebele, North")) );
	m_langList.append( new Language("nds"  , "Low German; Low Saxon" , QObject::tr("Low German; Low Saxon")) );
	m_langList.append( new Language("ne"  , "Nepali"      , QObject::tr("Nepali")) );
	//  m_langList.append( new Language("ng"  , "Ndonga"      , QObject::tr("Ndonga")) );
	m_langList.append( new Language("nl"  , "Dutch"       , QObject::tr("Dutch")) );
	m_langList.append( new Language("nn"  , "Norwegian Nynorsk"     , QObject::tr("Norwegian Nynorsk")) );
	m_langList.append( new Language("no"  , "Norwegian"   , QObject::tr("Norwegian")) );
	//  m_langList.append( new Language("nr"  , "Ndebele, South"     , QObject::tr("Ndebele, South")) );
	//  m_langList.append( new Language("nv"  , "Navajo"      , QObject::tr("Navajo")) );
	//  m_langList.append( new Language("ny"  , "Chichewa; Nyanja"      , QObject::tr("Chichewa; Nyanja")) );
	//  m_langList.append( new Language("oc"  , "Occitan (post 1500); Provençal"     , QObject::tr("Occitan (post 1500); Provençal")) );
	//  m_langList.append( new Language("om"  , "Oromo"       , QObject::tr("Oromo")) );
	//  m_langList.append( new Language("or"  , "Oriya"       , QObject::tr("Oriya")) );
	//  m_langList.append( new Language("os"  , "Ossetian; Ossetic"     , QObject::tr("Ossetian; Ossetic")) );
	//  m_langList.append( new Language("pa"  , "Panjabi"     , QObject::tr("Panjabi")) );
	m_langList.append( new Language("pap"  , "Papiamento"     , QObject::tr("Papiamento")) );
	//  m_langList.append( new Language("pi"  , "Pali"        , QObject::tr("Pali")) );
	m_langList.append( new Language("ppk"  , "Uma"    , QObject::tr("Uma")) );
	m_langList.append( new Language("pl"  , "Polish"      , QObject::tr("Polish")) );
	m_langList.append( new Language("pot"  , "Potawatomi"     , QObject::tr("Potawatomi")) );
	//  m_langList.append( new Language("ps"  , "Pushto"      , QObject::tr("Pushto")) );
	m_langList.append( new Language("pt"  , "Portuguese"  , QObject::tr("Portuguese")) );
	m_langList.append( new Language("pt_BR"  , "Brasilian Portuguese"  , QObject::tr("Brasilian Portuguese")) );//added by ourself
	//  m_langList.append( new Language("qu"  , "Quechua"     , QObject::tr("Quechua")) );
	//  m_langList.append( new Language("rm"  , "Raeto-Romance"    , QObject::tr("Raeto-Romance")) );
	//  m_langList.append( new Language("rn"  , "Rundi"       , QObject::tr("Rundi")) );
	m_langList.append( new Language("ro"  , "Romanian"    , QObject::tr("Romanian")) );
	m_langList.append( new Language("ru"  , "Russian"     , QObject::tr("Russian")) );
	//  m_langList.append( new Language("rw"  , "Kinyarwanda" , QObject::tr("Kinyarwanda")) );
	//  m_langList.append( new Language("sa"  , "Sanskrit"    , QObject::tr("Sanskrit")) );
	//  m_langList.append( new Language("sc"  , "Sardinian"   , QObject::tr("Sardinian")) );
	m_langList.append( new Language("sco"  , "Scots"   , QObject::tr("Scots")) );
	//  m_langList.append( new Language("sd"  , "Sindhi"      , QObject::tr("Sindhi")) );
	//  m_langList.append( new Language("se"  , "Northern Sami" , QObject::tr("Northern Sami")) );
	//  m_langList.append( new Language("sg"  , "Sango"       , QObject::tr("Sango")) );
	//  m_langList.append( new Language("si"  , "Sinhalese"   , QObject::tr("Sinhalese")) );
	m_langList.append( new Language("sk"  , "Slovak"      , QObject::tr("Slovak")) );
	m_langList.append( new Language("sl"  , "Slovenian"   , QObject::tr("Slovenian")) );
	//  m_langList.append( new Language("sm"  , "Samoan"      , QObject::tr("Samoan")) );
	//  m_langList.append( new Language("sn"  , "Shona"       , QObject::tr("Shona")) );
	m_langList.append( new Language("so"  , "Somali"      , QObject::tr("Somali")) );
	m_langList.append( new Language("sq"  , "Albanian"    , QObject::tr("Albanian")) );
	//  m_langList.append( new Language("sr"  , "Serbian"     , QObject::tr("Serbian")) );
	m_langList.append( new Language("srn"  , "Sranan"    , QObject::tr("Sranan")) );
	//  m_langList.append( new Language("ss"  , "Swati"       , QObject::tr("Swati")) );
	//  m_langList.append( new Language("st"  , "Sotho, Southern"     , QObject::tr("Sotho, Southern")) );
	//  m_langList.append( new Language("su"  , "Sundanese"   , QObject::tr("Sundanese")) );
	m_langList.append( new Language("sv"  , "Swedish"     , QObject::tr("Swedish")) );
	m_langList.append( new Language("sw"  , "Swahili"     , QObject::tr("Swahili")) );
	m_langList.append( new Language("syr"  , "Syriac"     , QObject::tr("Syriac")) );
	m_langList.append( new Language("ta"  , "Tamil"       , QObject::tr("Tamil")) );
	//  m_langList.append( new Language("te"  , "Telugu"      , QObject::tr("Telugu")) );
	//  m_langList.append( new Language("tg"  , "Tajik"       , QObject::tr("Tajik")) );
	m_langList.append( new Language("th"  , "Thai"        , QObject::tr("Thai")) );
	//  m_langList.append( new Language("tk"  , "Turkmen"     , QObject::tr("Turkmen")) );
	m_langList.append( new Language("tl"  , "Tagalog"     , QObject::tr("Tagalog")) );
	m_langList.append( new Language("tlh"  , "Klingon"    , QObject::tr("Klingon")) );
	m_langList.append( new Language("tn"  , "Tswana"      , QObject::tr("Tswana")) );
	m_langList.append( new Language("tr"  , "Turkish"     , QObject::tr("Turkish")) );
	//  m_langList.append( new Language("ts"  , "Tsonga"      , QObject::tr("Tsonga")) );
	//  m_langList.append( new Language("tt"  , "Tatar"       , QObject::tr("Tatar")) );
	//  m_langList.append( new Language("tw"  , "Twi"         , QObject::tr("Twi")) );
	m_langList.append( new Language("ty"  , "Tahitian"    , QObject::tr("Tahitian")) );
	//  m_langList.append( new Language("ug"  , "Uighur"      , QObject::tr("Uighur")) );
	m_langList.append( new Language("uk"  , "Ukrainian"   , QObject::tr("Ukrainian")) );
	//  m_langList.append( new Language("ur"  , "Urdu"        , QObject::tr("Urdu")) );
	//  m_langList.append( new Language("uz"  , "Uzbek"       , QObject::tr("Uzbek")) );
	m_langList.append( new Language("vi"  , "Vietnamese"  , QObject::tr("Vietnamese")) );
	//  m_langList.append( new Language("vo"  , "Volapük"     , QObject::tr("Volapük")) );
	//  m_langList.append( new Language("wo"  , "Wolof"       , QObject::tr("Wolof")) );
	m_langList.append( new Language("xh"  , "Xhosa"       , QObject::tr("Xhosa")) );
	m_langList.append( new Language("x-E-BAR"  , "Bavarian"     , QObject::tr("Bavarian")) );
	m_langList.append( new Language("x-E-DJE"  , "Zarma"     , QObject::tr("Zarma")) );
	m_langList.append( new Language("x-E-GSW"  , "Alemannisch"  , QObject::tr("Alemannisch")) );
	m_langList.append( new Language("x-E-HAT"  , "Haitian Creole French" , QObject::tr("Haitian Creole French")) );
	m_langList.append( new Language("x-E-ITZ"  , QString::fromUtf8("Itzá")         , QObject::tr("Itzá")) );
	m_langList.append( new Language("x-E-JIV"  , "Shuar"        , QObject::tr("Shuar")) );
	m_langList.append( new Language("x-E-KEK"  , QString::fromUtf8("Kekchí")       , QObject::tr("Kekchí")) );
	m_langList.append( new Language("x-E-KAB"  , "Kabyle"      , QObject::tr("Kabyle")) );
	m_langList.append( new Language("x-E-LMO"  , "Lombard"      , QObject::tr("Lombard")) );
	m_langList.append( new Language("x-E-MKJ"  , "Macedonian"   , QObject::tr("Macedonian")) );
	m_langList.append( new Language("x-E-PDG"  , "Tok Pisin"    , QObject::tr("Tok Pisin")) );
	m_langList.append( new Language("x-E-PPK"  , "Uma"          , QObject::tr("Uma")) );
	m_langList.append( new Language("x-E-RMY"  , "Romani, Vlax" , QObject::tr("Romani, Vlax")) );
	m_langList.append( new Language("x-E-SAJ"  , "Sango"        , QObject::tr("Sango")) );
	m_langList.append( new Language("x-E-SRN"  , "Sranan"       , QObject::tr("Sranan")) );
	m_langList.append( new Language("yi"  , "Yiddish"     , QObject::tr("Yiddish")) );
	m_langList.append( new Language("yo"  , "Yoruba"     , QObject::tr("Yoryba")) );
	//  m_langList.append( new Language("za"  , "Zhuang"      , QObject::tr("Zhuang")) );
	m_langList.append( new Language("zh"  , "Chinese"     , QObject::tr("Chinese")) );
	m_langList.append( new Language("zu"  , "Zulu"        , QObject::tr("Zulu")) );

	foreach (Language* lang, m_langList) {
		m_langMap.insert( lang->abbrev(), lang);
	}
}
