/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CBOOKSHELFINDEX_H
#define CBOOKSHELFINDEX_H

//BibleTime includes
#include "btindexitem.h"
#include "backend/btmoduletreeitem.h"
class CSwordModuleInfo;
#include "frontend/displaywindow/cdisplaywindow.h"

#include "util/cpointers.h"

//Qt includes
#include <QTimer>
#include <QList>
#include <QTreeWidget>
#include <QTreeWidgetItem>



class CSearchDialog;
class CMainIndex;
class QWidget;
class QDropEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class BTMimeData;
class QMenu;
class QActionGroup;
class QMouseEvent;
class BTModuleTreeItem;



/**
* The widget which manages the modules. The modules are put into own,
* fixed subfolders sorted by category and/or language.
* @author The BibleTime team
*/
class CBookshelfIndex : public QTreeWidget {
	Q_OBJECT

public:
	CBookshelfIndex(QWidget *parent);
	virtual ~CBookshelfIndex();
	
	void initTree();
	/**
	* Opens the searchdialog using the given modules using the given search text.
	*/
	void emitModulesChosen( QList<CSwordModuleInfo*> modules, QString key );

	/** Helper method for "freezing" modules while opening them in windows. */
	void unfreezeModules(QList<CSwordModuleInfo*>);

public slots:

	/**
	* Reloads the main index's Sword dependend things like modules
	*/
	void reloadSword(CSwordBackend::SetupChangedReason);

	/**
	 * Opens the searchdialog for the selected modules.
	 */
	void actionSearchInModules();

protected:

	/** Handle mouse clicks.*/
	virtual void mouseReleaseEvent(QMouseEvent* event);

	/** Empty re-implementation to prevent unwanted opening of folders with some styles.*/
	virtual void mouseDoubleClickEvent(QMouseEvent*) {}

	/**
	* Initialize the SIGNAL<->SLOT connections
	*/
	void initConnections();
	/**
	* Reimplementation. Returns the drag object for the current selection.
	*/
	virtual QMimeData* dragObject();
	/**
	* Reimplementation from QTreeWidget (QAbstractItemView). Returns true if the drag is acceptable for the listview.
	*/
	virtual void dragEnterEvent( QDragEnterEvent* event );
	virtual void dragMoveEvent( QDragMoveEvent* event );
	virtual void dropEvent( QDropEvent* event );

	/** Receive style change event to change single/double click activation behaviour. */
	virtual void changeEvent(QEvent*);
	
	/**
	* Reimplementation from QAbstractItemView. Takes care of movable items.
	*/
	virtual void startDrag(Qt::DropActions supportedActions);
	/**
	* TODO: qt4 Reimplementation to support the items dragEnter and dragLeave functions.
	*/
	virtual void contentsDragMoveEvent( QDragMoveEvent* event );
	/**
	 * Reimplementation.
	 */
	virtual void contentsDragLeaveEvent( QDragLeaveEvent* e );
	

protected slots: // Protected slots
	/**
	* Called when the selection is changed.
	*/
	void slotModifySelection();
	/**
	* Is called when an item was clicked or activated.
	*/
	void slotExecuted( QTreeWidgetItem* );
	
	/**
	* Shows the context menu at the given position.
	*/
	void contextMenu(const QPoint&);

	/**
	* Changes the grouping.
	*/
	void actionChangeGrouping(QAction* action);

	/**
	* Shows information about the current module.
	*/

	/**
	* Show or hide the hidden modules, depending on the action check state.
	*/
	void actionShowModules(bool checked);

	/**
	* Set the selected modules hidden.
	*/
	void actionHideModules();


	void actionAboutModule();
	/**
	* Unlocks the current module.
	*/
	void actionUnlockModule();
	void autoOpenTimeout();
	
	/**
	* Opens a plain text editor window to edit the modules content.
	*/
	void actionEditModulePlain();
	/**
	* Opens an HTML editor window to edit the modules content.
	*/
	void actionEditModuleHTML();

	void addToTree(BTModuleTreeItem* item, QTreeWidgetItem* widgetItem);


private:
	CSearchDialog* m_searchDialog;
	//ToolTip* m_toolTip;
	bool m_itemsMovable;
	QTreeWidgetItem* m_autoOpenFolder;
	QTimer m_autoOpenTimer;
	BTModuleTreeItem::Grouping m_grouping; //temporary solution - this should be in config
	QActionGroup* m_groupingGroup;
	int m_mouseReleaseEventModifiers;
	bool m_showHidden;
	QSet<QString> m_frozenModules;

	/**
	* Initializes the view.
	*/
	void initView();
	void initActions();

	/** Convenience function for creating a new QAction. */
	QAction* newQAction(const QString& text, const QString& pix, int shortcut, const QObject* receiver, const char* slot, QObject* parent);

	QList<QAction*> m_actionList;

	QMenu* m_popup;

signals:
	/**
	* Is emitted when a module should be opened,
	*/
	void createReadDisplayWindow( QList<CSwordModuleInfo*>, const QString& );
	/**
	 * Is emitted when a write window should be created.
	 */
	void createWriteDisplayWindow( CSwordModuleInfo*, const QString&, const CDisplayWindow::WriteWindowType& );

};

#endif
