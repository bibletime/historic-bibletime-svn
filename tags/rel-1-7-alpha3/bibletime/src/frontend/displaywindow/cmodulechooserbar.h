/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CMODULECHOOSERBAR_H
#define CMODULECHOOSERBAR_H

//BibleTime includes
#include "backend/drivers/cswordmoduleinfo.h"

#include "util/cpointers.h"

//Qt includes
#include <QList>
#include <QToolBar>

class CModuleChooserButton;
class QWidget;

/**
  * @author The BibleTime team
  */
class CModuleChooserBar : public QToolBar  {
	Q_OBJECT
public:
	/**
	* Default constructor
	*/
	CModuleChooserBar(QList<CSwordModuleInfo*> useModules, CSwordModuleInfo::ModuleType type,  QWidget *parent=0);
	/**
	* Returns a list of selected modules.
	*/
	QList<CSwordModuleInfo*> getModuleList();
	/**
	* Sets the number of the maximum count of buttons.
	*/
	void setButtonLimit( const int limit);
	/**
	* Sets the modules which are chosen in this module chooser bar.
	*/
	void setModules( QList<CSwordModuleInfo*> modules );

signals:
	void sigChanged();

protected:
	/**
	 * Adds a button to the toolbar
	 */
	CModuleChooserButton* const addButton( CSwordModuleInfo* const module );

protected slots: // Protected slots
	/* 
	* This slot calls the addButton function above to add a button.
	*/
	void addButton();
	/**
	* Removes a button from the toolbar
	*/
	void removeButton( const int ID );
	/** */
	void updateMenuItems();

private:
	CSwordModuleInfo::ModuleType m_moduleType;
	int m_idCounter;
	int m_buttonLimit;
	QList<CModuleChooserButton*> m_buttonList;

};

#endif
