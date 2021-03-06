/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2009 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "btshortcutsdialog.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QLabel>
#include <QDialogButtonBox>
#include <QKeyEvent>

// *************** BtShortcutsDialog ***************************************************************************
// A dialog to allow the user to input a shortcut for a primary and alternate key

// dialog constructor
BtShortcutsDialog::BtShortcutsDialog(QWidget* parent)
	: QDialog(parent), m_primaryLabel(0), m_alternateLabel(0), m_primaryButton(0), m_alternateButton(0)
{
	setWindowTitle(tr("Configure shortcuts"));
	setMinimumWidth(350);

	QVBoxLayout* vLayout = new QVBoxLayout(this);
	setLayout(vLayout);

	QGridLayout* gridLayout = new QGridLayout();
	vLayout->addLayout(gridLayout);

	m_primaryButton = new QRadioButton(tr("First shortcut"));	
	m_primaryButton->setChecked(true);
	gridLayout->addWidget(m_primaryButton, 0, 0);

	m_alternateButton = new QRadioButton(tr("Second shortcut"));	
	gridLayout->addWidget(m_alternateButton, 1, 0);

	m_primaryLabel = new QLabel();
	m_primaryLabel->setMinimumWidth(100);
	m_primaryLabel->setFrameShape(QFrame::Panel);
	gridLayout->addWidget(m_primaryLabel, 0, 1);

	m_alternateLabel = new QLabel();
	m_alternateLabel->setMinimumWidth(100);
	m_alternateLabel->setFrameShape(QFrame::Panel);
	gridLayout->addWidget(m_alternateLabel, 1, 1);

	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	vLayout->addWidget(buttons);

	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

// get new primary key from dialog
QString BtShortcutsDialog::getFirstKeys()
{
	return m_primaryLabel->text();
}

// set the initial value of the primary key
void BtShortcutsDialog::setFirstKeys(const QString& keys)
{
	m_primaryLabel->setText(keys);
}

	// get new second keys from dialog
QString BtShortcutsDialog::getSecondKeys()
{
	return m_alternateLabel->text();
}

// set the initial value of the second keys
void BtShortcutsDialog::setSecondKeys(const QString& keys)
{
	m_alternateLabel->setText(keys);
}

// get key from users input, put into primary or alternate label for display to user
void BtShortcutsDialog::keyReleaseEvent(QKeyEvent* event)
{
	int key = event->key();
	if ( (key == Qt::Key_Shift) || (key == Qt::Key_Control) || (key == Qt::Key_Meta) || (key == Qt::Key_Alt) )
		return;

	QKeySequence keys(key);
	QString keyStr = keys.toString();
	if ( (event->modifiers() & Qt::AltModifier) == Qt::AltModifier)
		keyStr = "Alt+" + keyStr;
	if ( (event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier)
		keyStr = "Shift+" + keyStr;
	if ( (event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
		keyStr = "Ctrl+" + keyStr;
	
	QKeySequence completeKeys(keyStr);
	QString completeStr = completeKeys.toString();

	keyChangeRequest(completeStr);
}

// complete the keyChangeRequest
void BtShortcutsDialog::changeSelectedShortcut(const QString& keys)
{
	if (m_primaryButton->isChecked())
		m_primaryLabel->setText(keys);

	if (m_alternateButton->isChecked())
		m_alternateLabel->setText(keys);
}


