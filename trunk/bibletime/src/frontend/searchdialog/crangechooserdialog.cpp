/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2007 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



#include "crangechooserdialog.h"
#include "crangechooserdialog.moc"

#include "frontend/cbtconfig.h"

//sword
#include "versekey.h"
#include "listkey.h"


#include <klocale.h>

#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QDialogButtonBox>

namespace Search {
	namespace Options {

/********** RangeItem ******************/
CRangeChooserDialog::RangeItem::RangeItem(QListWidget* view, QListWidgetItem* afterThis, const QString caption, const QString range) 
	: QListWidgetItem(view)
{
	setCaption(caption);
	setRange(range);
}

CRangeChooserDialog::RangeItem::~RangeItem() {}

const QString& CRangeChooserDialog::RangeItem::range()
{
	//  qWarning("range is %s", (const char*)m_range.utf8());
	return m_range;
}

void CRangeChooserDialog::RangeItem::setRange(QString newRange) {
	m_range = newRange;
}

const QString CRangeChooserDialog::RangeItem::caption() {
	return text();
}

void CRangeChooserDialog::RangeItem::setCaption(const QString newCaption) {
	setText(newCaption);
}


/*********** Dialog ***************/

CRangeChooserDialog::CRangeChooserDialog( QWidget* parentDialog )
	: QDialog(parentDialog)
	//qdialog constr: (Plain, i18n("Search range editor"), Default | Ok | Cancel, Ok, parentDialog, "CRangeChooserDialog", false, true)
{
	initView();
	initConnections();

	//add the existing scopes
	CBTConfig::StringMap map = CBTConfig::get
								   (CBTConfig::searchScopes);
	CBTConfig::StringMap::Iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
		new RangeItem(m_rangeList, 0, it.key(), it.value());
	};

	editRange(0);
	if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem())
	   ) {
		nameChanged(i->caption());
	}
}

/** Initializes the view of this object. */
void CRangeChooserDialog::initView()
{
	//designed with qdesigner, code taken from the generated .h file
	
	//QSize size(576, 589);
	//size = size.expandedTo(crangechooserdialog->minimumSizeHint());
	//this->resize(size);
	QVBoxLayout* vboxLayout = new QVBoxLayout(this);

	QHBoxLayout* hboxLayout = new QHBoxLayout();
	QVBoxLayout* vboxLayout1 = new QVBoxLayout();
	QLabel* rangeListLabel = new QLabel(this);
	vboxLayout1->addWidget(rangeListLabel);

	m_rangeList = new QListWidget(this);
	vboxLayout1->addWidget(m_rangeList);

	QHBoxLayout* hboxLayout1 = new QHBoxLayout();
	m_newRangeButton = new QPushButton(this);
	hboxLayout1->addWidget(m_newRangeButton);
	m_deleteRangeButton = new QPushButton(this);
	hboxLayout1->addWidget(m_deleteRangeButton);

	vboxLayout1->addLayout(hboxLayout1);
	hboxLayout->addLayout(vboxLayout1);

	QVBoxLayout* vboxLayout2 = new QVBoxLayout();
	QHBoxLayout* hboxLayout2 = new QHBoxLayout();
	QLabel* nameEditLabel = new QLabel(this);
	hboxLayout2->addWidget(nameEditLabel);

	m_nameEdit = new QLineEdit(this);
	hboxLayout2->addWidget(m_nameEdit);
	vboxLayout2->addLayout(hboxLayout2);

	QLabel* rangeEditLabel = new QLabel(this);
	vboxLayout2->addWidget(rangeEditLabel);

	m_rangeEdit = new QTextEdit(this);

	vboxLayout2->addWidget(m_rangeEdit);

	QLabel* resultListLabel = new QLabel(this);
    vboxLayout2->addWidget(resultListLabel);

	m_resultList = new QListWidget(this);
	vboxLayout2->addWidget(m_resultList);

	hboxLayout->addLayout(vboxLayout2);
	vboxLayout->addLayout(hboxLayout);

	QFrame* line = new QFrame(this);
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	vboxLayout->addWidget(line);

    m_buttonBox = new QDialogButtonBox(this);
    m_buttonBox->setOrientation(Qt::Horizontal);
    m_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok|QDialogButtonBox::RestoreDefaults);
    vboxLayout->addWidget(m_buttonBox);

    rangeListLabel->setBuddy(m_rangeList);
    nameEditLabel->setBuddy(m_nameEdit);
    rangeEditLabel->setBuddy(m_rangeEdit);
    resultListLabel->setBuddy(m_resultList);
}



void CRangeChooserDialog::initConnections()
{
	// Signals from text/list widgets
/*	QObject::connect(m_rangeList, SIGNAL(selectionChanged(QListWidgetItem*)),
			this, SLOT(editRange(QListWidgetItem*)));

	QObject::connect(m_rangeEdit, SIGNAL(textChanged()),
			this, SLOT(parseRange()));
	QObject::connect(m_rangeEdit, SIGNAL(textChanged()),
			this, SLOT(rangeChanged()));

	QObject::connect(m_nameEdit, SIGNAL(textChanged(const QString&)),
			this, SLOT(nameChanged(const QString&)));
	*/
	// Buttons
	//QObject::connect(m_buttonBox, SIGNAL(accepted()), crangechooserdialog, SLOT(slotOk()));
	//don't use... QObject::connect(m_buttonBox, SIGNAL(rejected()), crangechooserdialog, SLOT(reject()));
	//connect(m_newRangeButton, SIGNAL(clicked()), this, SLOT(addNewRange()));
	//connect(m_deleteRangeButton, SIGNAL(clicked()), this, SLOT(deleteCurrentRange()));
	//restore defaults!

}

/** Adds a new range to the list. */
void CRangeChooserDialog::addNewRange()
{
	//RangeItem* i = new RangeItem(m_rangeList, m_rangeList->lastItem(), i18n("New range"));
	RangeItem* i = new RangeItem(m_rangeList, 0, i18n("New range"));
	//use just setCurrentItem... m_rangeList->setSelected(i, true);
	m_rangeList->setCurrentItem(i);
	editRange(i);

	m_nameEdit->setFocus();
}

/** No descriptions */
void CRangeChooserDialog::editRange(QListWidgetItem* item) {
	RangeItem* const range = dynamic_cast<RangeItem*>(item);

	m_nameEdit->setEnabled( range ); //only if an item is selected enable the edit part
	m_rangeEdit->setEnabled( range );
	m_resultList->setEnabled( range );
	m_deleteRangeButton->setEnabled( range );

	if (range) {
		m_nameEdit->setText(range->caption());
		m_rangeEdit->setText(range->range());
	}
}

/** Parses the entered text and prints out the result in the list box below the edit area. */
void CRangeChooserDialog::parseRange() {
	m_resultList->clear();

	//TODO: remove this hack:
	//HACK: repair range to  work with Sword 1.5.6
	QString range( m_rangeEdit->toPlainText() );
	range.replace(QRegExp("\\s{0,}-\\s{0,}"), "-" );

	sword::VerseKey key;
	sword::ListKey verses = key.ParseVerseList((const char*)range.toUtf8(), "Genesis 1:1", true);
	for (int i = 0; i < verses.Count(); ++i) {
		new QListWidgetItem(QString::fromUtf8(verses.GetElement(i)->getRangeText()), m_resultList );
		//    qWarning("range=%s, text=%s",verses.GetElement(i)->getRangeText(), verses.GetElement(i)->getText() );
	}

}

/** No descriptions */
void CRangeChooserDialog::rangeChanged() {
	if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem())
	   ) {
		QString range( m_rangeEdit->toPlainText() );
		//TODO: remove this hack:
		//HACK: repair range to work with Sword 1.5.6
		range.replace(QRegExp("\\s{0,}-\\s{0,}"), "-" );
		i->setRange(range);
	};
}

/** No descriptions */
void CRangeChooserDialog::nameChanged(const QString& newCaption) {
	m_rangeEdit->setEnabled(!newCaption.isEmpty());
	m_resultList->setEnabled(!newCaption.isEmpty());
	//m_resultList->header()->setEnabled(!newCaption.isEmpty());

	if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem())
	   ) {
		if (!newCaption.isEmpty()) {
			m_newRangeButton->setEnabled(true);
			i->setCaption(newCaption);
			m_rangeList->sortItems();
		}
		else { //invalid name
			i->setCaption(i18n("<invalid name of search range>"));
			m_newRangeButton->setEnabled(false);
		};
	};
}

/** Deletes the selected range. */
void CRangeChooserDialog::deleteCurrentRange() {
// 	if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem())
// 	   ) {
// 		if (QListWidgetItem* selection = i->itemBelow() ? i->itemBelow() : i->itemAbove()) {
// 			m_rangeList->setSelected(selection, true);
// 			m_rangeList->setCurrentItem(selection);
// 		}
// 		else {
// 			m_rangeList->setSelected(m_rangeList->firstChild(), true);
// 			m_rangeList->setCurrentItem(m_rangeList->firstChild());
// 		}
// 		delete i;
// 	}
// 	editRange(m_rangeList->currentItem());
}

void CRangeChooserDialog::slotOk() {
	//save the new map of search scopes
	CBTConfig::StringMap map;
	//QListWidgetItemIterator it( m_rangeList );
	//for (;it.current(); ++it) {
	//	if ( RangeItem* i = dynamic_cast<RangeItem*>(it.current()) ) {
	//		map[i->caption()] = i->range();
	//	};
	//};
	for (int i = 0; i < m_rangeList->count(); i++){
		if ( RangeItem* item = dynamic_cast<RangeItem*>(m_rangeList->item(i)) ) {
			map[item->caption()] = item->range();
		};
	}
	CBTConfig::set
		(CBTConfig::searchScopes, map);

	//KDialogBase::slotOk();
	QDialog::accept();
}

void CRangeChooserDialog::slotDefault() {
	m_rangeList->clear();
	CBTConfig::StringMap map = CBTConfig::getDefault(CBTConfig::searchScopes);
	CBTConfig::StringMap::Iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
		new RangeItem(m_rangeList, 0, it.key(), it.value());
	};
	//m_rangeList->setSelected(m_rangeList->selectedItem(), false);
	m_rangeList->setCurrentItem(0);

	editRange(0);
	if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem())
	   ) {
		nameChanged(i->caption());
	}

	//KDialogBase::slotDefault();
}

	} //end of namespace Search.Options
} //end of namespace Search
