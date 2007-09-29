//
// C++ Implementation: cmodulechooserdialog
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "cmodulechooserdialog.h"
#include "cmodulechooserdialog.moc"

#include "backend/drivers/cswordmoduleinfo.h"
#include "backend/managers/cswordbackend.h"
#include "util/cpointers.h"
#include "util/cresmgr.h"
#include "util/ctoolclass.h"
#include "util/directoryutil.h"

#include <QDialog>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QStringList>

#include <klocale.h>

namespace Search { namespace Options {

CModuleChooserDialog::CModuleChooserDialog( QWidget* parentDialog, ListCSwordModuleInfo modules )
	: QDialog(parentDialog)
	//KDialogBase(Plain, i18n("Choose work(s)"), Ok, Ok, parentDialog, "CModuleChooser", false, true)
{
	setWindowTitle(i18n("Choose work(s)"));
	initView();
	initConnections();

	setModules(modules);
};

CModuleChooserDialog::~CModuleChooserDialog() {}
;

/** Initializes the view of this dialog */
void CModuleChooserDialog::initView()
{
	//TODO: choose the button text
	//setButtonOKText(i18n("Use chosen work(s)"));
	QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;

	vboxLayout = new QVBoxLayout(this);

    m_moduleChooser = new QTreeWidget(this);
    vboxLayout->addWidget(m_moduleChooser);

    hboxLayout = new QHBoxLayout();

    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hboxLayout->addItem(spacerItem);

    m_buttonBox = new QDialogButtonBox(this);
    m_buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    hboxLayout->addWidget(m_buttonBox);

    vboxLayout->addLayout(hboxLayout);

}

void CModuleChooserDialog::setModules(ListCSwordModuleInfo& selectedModules)
{
	ListCSwordModuleInfo mods = CPointers::backend()->moduleList();
	qDebug("CModuleChooserDialog::setModules");
	//Populate the chooser widget with items
	/**
	* The next steps:
	* 1. Sort by type
	* 2. Sort the modules of this type by their language
	* 3. Create the subfolders for this
	*/

	QList<QTreeWidgetItem*> topLevelFolders;

	QMap<CSwordModuleInfo::ModuleType, QString> typenameMap;
	typenameMap.insert(CSwordModuleInfo::Bible, i18n("Bibles"));
	typenameMap.insert(CSwordModuleInfo::Commentary, i18n("Commentaries"));
	typenameMap.insert(CSwordModuleInfo::Lexicon, i18n("Lexicons"));
	typenameMap.insert(CSwordModuleInfo::GenericBook, i18n("Books"));

	int type = CSwordModuleInfo::Bible;
	bool ok = true;
	bool addedDevotionals = false;
	bool addedGlossaries = false;
	bool addedLexs = false;
	bool incType = false;

	while (ok) {
		qDebug("beginning of while(ok)");
		ListCSwordModuleInfo modsForType;
		QString typeFolderCaption = QString::null;
		incType = false;
		if (static_cast<CSwordModuleInfo::ModuleType>(type) == CSwordModuleInfo::Lexicon) {
			if (!addedLexs) {
				//         for (mods.first(); mods.current(); mods.next()) {
				ListCSwordModuleInfo::iterator end_it = mods.end();
				for (ListCSwordModuleInfo::iterator it(mods.begin()); it != end_it; ++it) {
					if (((*it)->type() == CSwordModuleInfo::Lexicon)
							&& ((*it)->category() != CSwordModuleInfo::DailyDevotional)
							&& ((*it)->category() != CSwordModuleInfo::Glossary)
					   ) {
						modsForType.append( *it );
					}
				}

				addedLexs = true;
				typeFolderCaption = QString::null;
			}
			else if (!addedDevotionals) {
				//         for (mods.first(); mods.current(); mods.next()) {
				ListCSwordModuleInfo::iterator end_it = mods.end();
				for (ListCSwordModuleInfo::iterator it(mods.begin()); it != end_it; ++it) {
					if ((*it)->category() == CSwordModuleInfo::DailyDevotional) {
						modsForType.append(*it);
					}
				}
				addedDevotionals = true;
				typeFolderCaption = i18n("Daily Devotionals");
			}
			else if (!addedGlossaries) {
				//         for (mods.first(); mods.current(); mods.next()) {
				ListCSwordModuleInfo::iterator end_it = mods.end();
				for (ListCSwordModuleInfo::iterator it(mods.begin()); it != end_it; ++it) {
					if ((*it)->category() == CSwordModuleInfo::Glossary) {
						modsForType.append(*it);
					}
				}
				addedGlossaries = true;
				typeFolderCaption = i18n("Glossaries");
			};

			if (addedLexs && addedDevotionals && addedGlossaries)
				incType = true;
		}
		else if (type == CSwordModuleInfo::Bible || type == CSwordModuleInfo::Commentary || type == CSwordModuleInfo::GenericBook) {
			//       for (mods.first(); mods.current(); mods.next()) {
			ListCSwordModuleInfo::iterator end_it = mods.end();
			for (ListCSwordModuleInfo::iterator it(mods.begin()); it != end_it; ++it) {
				if ((*it)->type() == type) {
					modsForType.append(*it);
				}
			}
			incType = true;
		}
		else
			ok = false;

		if (typeFolderCaption.isEmpty()) {
			typeFolderCaption = typenameMap[static_cast<CSwordModuleInfo::ModuleType>(type)];
		}

		//get the available languages of the selected modules
		QStringList langs;
		//     for (modsForType.first(); modsForType.current(); modsForType.next()) {
		ListCSwordModuleInfo::iterator end_it = modsForType.end();
		for (ListCSwordModuleInfo::iterator it(modsForType.begin()); it != end_it; ++it) {
			if ( !langs.contains(QString( (*it)->module()->Lang() ))) {
				langs.append( (*it)->module()->Lang() );
			}
		}
		langs.sort();

		//go through the list of languages and create subfolders for each language and the modules of the language
		QTreeWidgetItem* typeFolder = 0;
		if (modsForType.count()) {
			qDebug("create a new typeFolder");
			typeFolder = new QTreeWidgetItem(typeFolder, QStringList(typeFolderCaption));
		}
		else {
			if (incType) {
				type++;
			}
			continue;
		}


		QString language = QString::null;
		CLanguageMgr* langMgr = languageMgr();
		for ( QStringList::Iterator it = langs.begin(); it != langs.end(); ++it ) {
			language = langMgr->languageForAbbrev(*it)->translatedName();
			if (language.isEmpty()) {
				language = (*it);
			}

			qDebug("create a new langFolder");
			QTreeWidgetItem* langFolder = new QTreeWidgetItem(typeFolder, QStringList(language));
			//langFolder->setPixmap(0, util::filesystem::DirectoryUtil::getIcon(CResMgr::mainIndex::closedFolder::icon));

			//create the module items of this lang folder
			//       for (modsForType.first(); modsForType.current(); modsForType.next()) {
			ListCSwordModuleInfo::iterator end_modItr = modsForType.end();
			for (ListCSwordModuleInfo::iterator mod_Itr(modsForType.begin()); mod_Itr != end_modItr; ++mod_Itr) {
				if (QString( (*mod_Itr)->module()->Lang() ) == (*it) ) { //found correct language
					//ModuleCheckBoxItem* i = new ModuleCheckBoxItem(langFolder, *mod_Itr);
					//i->setPixmap(0, CToolClass::getIconForModule(*mod_Itr));
					QTreeWidgetItem* i = new QTreeWidgetItem(langFolder, QStringList((*mod_Itr)->name()));
					i->setFlags(Qt::ItemIsUserCheckable);
					if (selectedModules.contains(*mod_Itr)) {
						//Module was in the list of already selected modules
						i->setCheckState(0, Qt::Checked);
					} else {
						i->setCheckState(0, Qt::Unchecked);
					}
				}
			}
		}
		//typeFolder->setPixmap(0, util::filesystem::DirectoryUtil::getIcon(CResMgr::mainIndex::closedFolder::icon));
		qDebug("add top level folder...");
		topLevelFolders.append(typeFolder);

		if (incType) {
			++type;
		}
	}

	//put these into m_moduleChooser
	m_moduleChooser->invisibleRootItem()->addChildren(topLevelFolders);
	QTreeWidgetItemIterator it(m_moduleChooser);
	qDebug("m_moduleChooser items:");
	while (*it) {
		qDebug() << *it;
		++it;
	}
}

/** Initializes the connections of this dialog. */
void CModuleChooserDialog::initConnections()
{
	//QObject::connect(this, SIGNAL(accepted()), this, SLOT(slotOk()) );
	QObject::connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(slotOk()) );
}

/* Emits the list of selected modules */
void CModuleChooserDialog::slotOk()
{
	//create the list of selected modules
	ListCSwordModuleInfo mods;
	QTreeWidgetItemIterator it( m_moduleChooser );
	for ( ; *it; ++it ) {
		//add the module to list if the box is checked
		if ((*it)->checkState(0) == Qt::Checked) {
			ListCSwordModuleInfo allMods = CPointers::backend()->moduleList();
			for (ListCSwordModuleInfo::iterator all_iter(allMods.begin()); all_iter != allMods.end(); ++all_iter) {
				if ((*all_iter)->name() == (*it)->text(0)) {
					mods.append(*all_iter);
					break;
				}
			}
			
		}
	}

	emit modulesChanged(mods);
	QDialog::done(0);
}

}} //end of namespaces Options and Search
