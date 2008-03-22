/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "btinstallpathdialog.h"
#include "btinstallpathdialog.moc"

#include "frontend/bookshelfmanager/new/backend.h"

#include "util/ctoolclass.h"

#include <QString>
#include <QDialog>
#include <QDir>
#include <QGridLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

BtInstallPathDialog::BtInstallPathDialog()
{
	//this->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(this);
	layout->setMargin(5);

	layout->setSpacing(10);
	layout->setColumnStretch(0,1);
	layout->setRowStretch(5,1);

	QLabel* mainLabel = CToolClass::explanationLabel(this,
						tr("Configure install paths"),
						tr("New works can be installed in one or more directories. After setting up directories here you can choose one of them in Install page.")
													);
	layout->addWidget(mainLabel, 0, 0, 1, 2);

	// TODO: use backend::
	QString swordConfPath = BTInstallMgr::Tool::LocalConfig::swordConfigFilename();
	QLabel* confPathLabel = new QLabel(tr("Configuration file for the paths is: ").append("<b>%1</b>").arg(swordConfPath), this);
	confPathLabel->setWordWrap(true);
	layout->addWidget(confPathLabel, 1, 0, 1, 2);

	m_swordPathListBox = new QTreeWidget(this);
	
	layout->addWidget(m_swordPathListBox, 2, 0, 4, 1);

	m_editButton = new QPushButton(tr("Edit..."), this);
	m_editButton->setIcon(util::filesystem::DirectoryUtil::getIcon("edit"));
	connect(m_editButton, SIGNAL(clicked()), this, SLOT(slotEditClicked()));
	layout->addWidget(m_editButton, 2, 1);

	m_addButton = new QPushButton(tr("Add..."), this);
	m_addButton->setIcon(util::filesystem::DirectoryUtil::getIcon("edit_add"));
	connect(m_addButton, SIGNAL(clicked()), this, SLOT(slotAddClicked()));
	layout->addWidget(m_addButton, 3, 1);

	m_removeButton = new QPushButton(tr("Remove"), this);
	m_removeButton->setIcon(util::filesystem::DirectoryUtil::getIcon("editdelete"));
	connect(m_removeButton, SIGNAL(clicked()), this, SLOT(slotRemoveClicked()));
	layout->addWidget(m_removeButton, 4, 1);
}

void BtInstallPathDialog::slotEditClicked() {
	if (QTreeWidgetItem* i = m_swordPathListBox->currentItem()) {
		QString dirname = QFileDialog::getExistingDirectory(this, tr("Choose directory"), i->text(0));
	if (dirname.isEmpty()) { // if user cancelled the dialog
		return;
	}
		QDir dir = QDir(dirname);
		if (dir.isReadable()) {
			const QFileInfo fi( dir.canonicalPath() );
			if (!fi.exists() || !fi.isWritable()) {
				const int result = QMessageBox::warning(this, tr("Confirmation"), tr("This directory is not writable, so works can not be installed here using BibleTime. Do you want to use this directory instead of the previous value?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
				if (result != QMessageBox::Yes) return;
			}
			i->setText(0, dir.canonicalPath());
			m_swordSetupChanged = true;
			writeSwordConfig(); //to make sure other parts work with the new setting
			populateInstallCombos(); //update target list bof on install page
			populateRemoveModuleListView();
		}
	}
}

void BtInstallPathDialog::slotAddClicked() {
	QString dirname = QFileDialog::getExistingDirectory(this, tr("Choose directory"), "");
	if (dirname.isEmpty()) { // if user cancelled the dialog
		return;
	}
	QDir dir = QDir(dirname);
	//qDebug() << "dir object:"<< dir.canonicalPath();
	if (dir.isReadable()) {
		const QFileInfo fi( dir.canonicalPath() );
		if (!fi.exists() || !fi.isWritable()) {
			const int result = QMessageBox::warning(this, tr("Warning"), tr("This directory is not writable, so works can not be installed here using BibleTime. Do you still want to add it to the list of bookshelf directories?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
			if (result != QMessageBox::Yes) {
				return;
			}
		}
		new QTreeWidgetItem(m_swordPathListBox, QStringList(dir.canonicalPath()) );
		m_swordSetupChanged = true;
		
		//TODO: signal is not needed, this is a modal dialog and the list can
		// be updated after closing this
		writeSwordConfig(); //to make sure other parts work with the new setting
		//populateInstallCombos();     //update target list bof on install page
		//populateRemoveModuleListView();
	}
}

void BtInstallPathDialog::slotRemoveClicked() {
	QTreeWidgetItem* i = m_swordPathListBox->currentItem();
	if (i) {
		delete i;

		// todo: see above
		m_swordSetupChanged = true;
		writeSwordConfig(); //to make sure other parts work with the new setting
		populateInstallCombos(); //update target list bof on install page
		populateRemoveModuleListView();
	}
}
