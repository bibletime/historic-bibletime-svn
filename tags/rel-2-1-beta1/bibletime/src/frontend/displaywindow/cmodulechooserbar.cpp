/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "cmodulechooserbar.h"

#include "cmodulechooserbutton.h"

#include <QList>
#include <QDebug>
#include <QAction>
#include <QToolBar>

CModuleChooserBar::CModuleChooserBar(QList<CSwordModuleInfo*> useModules, CSwordModuleInfo::ModuleType type, QWidget *parent)
	: QToolBar(parent),
	m_moduleType(type),
	m_idCounter(0),
	m_buttonLimit(-1) //-1 means no limit
{
	//insert buttons if useModules != 0
	QList<CSwordModuleInfo*>::iterator end_it = useModules.end();
	for (QList<CSwordModuleInfo*>::iterator it(useModules.begin()); it != end_it; ++it) {
		if ((m_buttonLimit != -1) && ( m_buttonLimit <= (int)m_buttonList.count()) ) { //we reached the button limit
			break;
		}
		addButton( *it );
	}

	// We can add a button to choose an additional module
	if ( (m_buttonLimit == -1) || (m_buttonLimit > (int)m_buttonList.count()) ) {
		addButton(0); //add a button without module set
	}
}

CModuleChooserButton* CModuleChooserBar::addButton( CSwordModuleInfo* const module )
{
	CModuleChooserButton* b = new CModuleChooserButton(module, m_moduleType, ++m_idCounter, this);
	QAction* a = addWidget(b);
	m_buttonList.append(b);
	connect( b, SIGNAL(sigAddButton()), this, SLOT(addButton()) );
	connect( b, SIGNAL(sigRemoveButton(const int)), this, SLOT(removeButton(const int)) );
	connect( b, SIGNAL(sigChanged()), SIGNAL(sigChanged()) );
	connect( b, SIGNAL(sigChanged()), SLOT(updateMenuItems()) );
	a->setVisible(true);
	updateMenuItems(); //make sure the items are up to date with the newest module list
	return b;
}

void CModuleChooserBar::addButton( ) {
	addButton(0);
}

//change current with next and remove
/** Removes a button from the toolbar */
void CModuleChooserBar::removeButton( const int ID ) {
	QMutableListIterator<CModuleChooserButton*> it(m_buttonList);
	while (it.hasNext()) {
		CModuleChooserButton* b = it.next();
		if (b->getId() == ID) { //found the right button to remove
			it.remove();
			b->deleteLater();
			break;
		}
	}
	emit sigChanged();
	updateMenuItems(); //make sure the items are up to date with the newest module list
}

/** Returns a list of selected modules. */
QList<CSwordModuleInfo*> CModuleChooserBar::getModuleList() {
	QList<CSwordModuleInfo*> list;
	foreach (CModuleChooserButton* b, m_buttonList)
	{
		if (b->module()) list.append( b->module() );
	}
	return list;
}

//change current with remove
/** Sets the number of the maximum count of buttons. */
void CModuleChooserBar::setButtonLimit(const int limit) {
	m_buttonLimit = limit;
	if (limit == -1) //no need to delete buttons
		return;

	const int tooMuch = m_buttonList.size() - limit;
	for (int i = 0; i < tooMuch; ++i) {
		CModuleChooserButton* b = m_buttonList.takeLast();
		b->deleteLater();
	}

	updateMenuItems();
}

/** Sets the modules which are chosen in this module chooser bar. */
void CModuleChooserBar::setModules( QList<CSwordModuleInfo*> useModules ) {
	setButtonLimit(0);
	setButtonLimit(-1);  //these two lines clear the bar

	if (!useModules.count()) return;

	QList<CSwordModuleInfo*>::iterator end_it = useModules.end();
	for (QList<CSwordModuleInfo*>::iterator it(useModules.begin()); it != end_it; ++it) {
		if ( (m_buttonLimit != -1) && (m_buttonLimit <= (int)m_buttonList.count()) ) {
			break;
		}
		addButton( *it );
	}

	if ( (m_buttonLimit == -1) || (m_buttonLimit > (int)m_buttonList.count()) ) {
		addButton(0);//add button without module set
	}

	updateMenuItems();
}

void CModuleChooserBar::updateMenuItems() {
	resize(sizeHint());
	update(); //seems to be neccessary to enforce display of the layout changes when a button was removed or added
	foreach (CModuleChooserButton* b, m_buttonList)
		b->updateMenuItems();
}
