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
#include <QFrame>
#include <QEvent>

BtConfigDialog::BtConfigDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::Window);
	m_contentsList = new QListWidget(this);
	m_contentsList->setViewMode(QListView::IconMode);
	m_contentsList->setMovement(QListView::Static);
	m_contentsList->setMaximumWidth(m_contentsList->minimumSizeHint().width());
	//m_contentsList->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
	m_contentsList->installEventFilter(this);

	m_pageWidget = new QStackedWidget(this);
	m_pageWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	setLayout(mainLayout);
	mainLayout->addWidget(m_contentsList);
	m_pageLayout = new QVBoxLayout;
	mainLayout->addLayout(m_pageLayout);
	
	m_pageLayout->addWidget(m_pageWidget);

	//line
	QFrame* line = new QFrame();
	line->setGeometry(QRect(1, 1, 1, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
	line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_pageLayout->addWidget(line);

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

	qDebug() << "item rect " << m_contentsList->visualItemRect(item);
	m_contentsList->setMaximumWidth(qMax(m_contentsList->maximumWidth(), (m_contentsList->visualItemRect(item).width() + 10 ) ) );
// 	int textWidth = m_contentsList->fontMetrics().width(item->text());
// 	qDebug() << "text width:"<< textWidth;
// 	qDebug() << "listview icon size:"<<m_contentsList->iconSize();
// 	qDebug() << "list maxwidth: " << m_contentsList->maximumWidth();
// 	if (m_contentsList->maximumWidth() < item->sizeHint().width()) {
// 		qDebug() << "set maxwidth: " << item->sizeHint().width();
// 		m_contentsList->setMaximumWidth(item->sizeHint().width());
// 	}
// 	m_contentsList->setMaximumWidth(qMax(m_contentsList->maximumWidth(), textWidth));
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

bool BtConfigDialog::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::Show) {
		qDebug("show event");
		qDebug() << m_contentsList->minimumSizeHint().width();
		for (int i = 0; i < m_contentsList->count(); i++) {
			qDebug()<< "size hint:"<< m_contentsList->item(i)->sizeHint().width();
			//m_contentsList->setMaximumWidth(qMax(m_contentsList->item(i)->sizeHint().width(), m_contentsList->maximumWidth() ) );
		}
		return true;
	} else {
		return QObject::eventFilter(obj, event);
	}
}

BtConfigPage::BtConfigPage() {}

BtConfigPage::~BtConfigPage() {}