/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "frontend/bookshelfmanager/btconfigdialog.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListView>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QVBoxLayout>
#include "util/directory.h"
#include "util/tool.h"


BtConfigDialog::BtConfigDialog(QWidget* parent)
        : QDialog(parent),
        m_maxItemWidth(0),
        m_previousPageIndex(-2) {
    setWindowFlags(Qt::Window);
    m_contentsList = new QListWidget(this);
    m_contentsList->setViewMode(QListView::IconMode);
    m_contentsList->setMovement(QListView::Static);

    m_pageWidget = new QStackedWidget(this);
    m_pageWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(m_contentsList);
    m_pageLayout = new QVBoxLayout;
    mainLayout->addLayout(m_pageLayout);

    m_pageLayout->addWidget(m_pageWidget);

    // Horizontal line
    QFrame* line = new QFrame();
    line->setGeometry(QRect(1, 1, 1, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pageLayout->addWidget(line);

    connect(m_contentsList,
            SIGNAL(currentRowChanged(int)),
            this, SLOT(slotChangePage(int))
           );

}

BtConfigDialog::~BtConfigDialog() {}

void BtConfigDialog::addPage(BtConfigPage* pageWidget) {
    namespace DU = util::directory;

    // this is a friend
    pageWidget->m_parentDialog = this;

    QVBoxLayout* containerLayout = new QVBoxLayout;
    QLabel* headerLabel = util::tool::explanationLabel(pageWidget, pageWidget->header(), pageWidget->label());
    containerLayout->addWidget(headerLabel);
    containerLayout->addWidget(pageWidget);
    QWidget* containerWidget = new QWidget(m_pageWidget);
    containerWidget->setLayout(containerLayout);
    m_pageWidget->addWidget(containerWidget);


    QListWidgetItem* item = new QListWidgetItem(m_contentsList);
    item->setIcon(DU::getIcon(pageWidget->iconName()));
    item->setText(pageWidget->header());
    item->setTextAlignment(Qt::AlignHCenter);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    //set the list width - it may bee too wide (if there were no pages) or too narrow
    if (m_maxItemWidth < m_contentsList->visualItemRect(item).width()) {
        m_maxItemWidth = m_contentsList->visualItemRect(item).width();
        m_contentsList->setFixedWidth( m_maxItemWidth + (m_contentsList->frameWidth()*2) );
    }
    // all items should has the same width
    for (int i = 0; i < m_contentsList->count(); ++i) {
        m_contentsList->item(i)->setSizeHint(QSize(m_maxItemWidth, m_contentsList->visualItemRect(m_contentsList->item(i)).height()) );
    }

    slotChangePage(m_contentsList->row(item));
}

void BtConfigDialog::addButtonBox(QDialogButtonBox* box) {
    m_pageLayout->addWidget(box);
}

void BtConfigDialog::slotChangePage(int newIndex) {
    if (m_previousPageIndex != newIndex) {
        m_previousPageIndex = newIndex;
        m_contentsList->setCurrentRow(newIndex);
        m_pageWidget->setCurrentIndex(newIndex);
    }
}



BtConfigPage::BtConfigPage(QWidget *parent)
    : QWidget(parent)
{
    // Intentionally empty
}

BtConfigPage::~BtConfigPage() {
    // Intentionally empty
}
