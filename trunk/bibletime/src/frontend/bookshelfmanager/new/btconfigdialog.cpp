/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/


#include "btconfigdialog.h"
#include "btconfigdialog.moc"

#include "util/directoryutil.h"
#include "util/ctoolclass.h"

#include <QDialog>
#include <QStackedWidget>
#include <QListWidget>
#include <QListView>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

BtConfigDialog::BtConfigDialog()
	: QDialog()
{
	m_contentsList = new QListWidget(this);
	m_contentsList->setViewMode(QListView::IconMode);
	m_contentsList->setMovement(QListView::Static);
	
	m_pageWidget = new QStackedWidget(this);
	
	QHBoxLayout *mainLayout = new QHBoxLayout;
	setLayout(mainLayout);
	mainLayout->addWidget(m_contentsList);
	m_pageLayout = new QVBoxLayout;
	mainLayout->addLayout(m_pageLayout);
	
	m_pageLayout->addWidget(m_pageWidget);
	
	connect(m_contentsList,
		SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
		this, SLOT(slotChangePage(QListWidgetItem *, QListWidgetItem*))
		);
	
}

BtConfigDialog::~BtConfigDialog() {}

void BtConfigDialog::addPage(BtConfigPage* pageWidget)
{
	
	QVBoxLayout* containerLayout = new QVBoxLayout;
	QLabel* headerLabel = CToolClass::explanationLabel(pageWidget, pageWidget->header(), pageWidget->label());
	containerLayout->addWidget(headerLabel);
	containerLayout->addWidget(pageWidget);
	QWidget* containerWidget = new QWidget(m_pageWidget);
	containerWidget->setLayout(containerLayout);
	m_pageWidget->addWidget(containerWidget);
	
	
	QListWidgetItem* item = new QListWidgetItem(m_contentsList);
	item->setIcon(util::filesystem::DirectoryUtil::getIcon(pageWidget->iconName()));
	item->setText(pageWidget->header());
	item->setTextAlignment(Qt::AlignHCenter);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void BtConfigDialog::addButtonBox(QDialogButtonBox* box)
{
	m_pageLayout->addWidget(box);
}

BtConfigPage* BtConfigDialog::currentPage()
{
	return dynamic_cast<BtConfigPage*>(m_pageWidget->currentWidget());
}

void BtConfigDialog::slotChangePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		current = previous;
	m_pageWidget->setCurrentIndex(m_contentsList->row(current));
}

BtConfigPage::BtConfigPage() {}

BtConfigPage::~BtConfigPage() {}