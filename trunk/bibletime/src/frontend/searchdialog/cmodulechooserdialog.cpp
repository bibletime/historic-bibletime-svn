/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2006 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "cmodulechooserdialog.h"
#include "cmodulechooserdialog.moc"

#include "backend/drivers/cswordmoduleinfo.h"
#include "backend/managers/cswordbackend.h"
#include "backend/btmoduletreeitem.h"

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
#include <QDebug>

#include <klocale.h>

namespace Search {

CModuleChooserDialog::CModuleChooserDialog( QWidget* parentDialog, ListCSwordModuleInfo modules )
	: QDialog(parentDialog)
	//KDialogBase(Plain, i18n("Choose work(s)"), Ok, Ok, parentDialog, "CModuleChooser", false, true)
{
	//Set the flag to destroy when closed - otherwise eats memory
	setAttribute(Qt::WA_DeleteOnClose);
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
	qDebug("CModuleChooserDialog::setModules");

	BTModuleTreeItem::HiddenOff hiddenFilter;
	QList<BTModuleTreeItem::Filter*> filters;
	filters.append(&hiddenFilter);
	BTModuleTreeItem root(filters, BTModuleTreeItem::CatLangMod);
	createModuleTree(&root, m_moduleChooser->invisibleRootItem());
	

	//put these into m_moduleChooser
	QTreeWidgetItemIterator it(m_moduleChooser);
	//qDebug("m_moduleChooser items:");
	//while (*it) {
	//	qDebug() << *it;
	//	++it;
	//}
}

void CModuleChooserDialog::createModuleTree(BTModuleTreeItem* item, QTreeWidgetItem* widgetItem)
{
	foreach (BTModuleTreeItem* i, item->children()) {
		createModuleTree(i, new QTreeWidgetItem(widgetItem));
	}
	if (item->type() != BTModuleTreeItem::Root) {
		widgetItem->setText(0, item->text());
		//TODO: set icon
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

} //end of namespace Search
