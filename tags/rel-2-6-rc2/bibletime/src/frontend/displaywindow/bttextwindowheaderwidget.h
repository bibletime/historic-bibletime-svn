/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BT_MODULECHOOSERHEADERWIDGET
#define BT_MODULECHOOSERHEADERWIDGET

#include <QString>
#include <QWidget>
#include <QList>

#include "backend/btmoduletreeitem.h"
#include "backend/drivers/cswordmoduleinfo.h"

class BtTextWindowHeader;
class QMenu;
class QAction;
class QLabel;
class QToolButton;
class QFrame;

/**
* A widget for choosing a module in a window. Consists of a label and a button.
* When user selects a module,
* button sends a signal. This widget needs to get a message back after a window
* module list has been changed. Only then it will be updated.
* See BtTextWindowHeader.
*/
class BtTextWindowHeaderWidget : public QWidget {
        Q_OBJECT

    public:
        /** For internal use to mark the menu items */
        enum TypeOfAction {RemoveAction, AddAction, ReplaceAction};

        /** Filter out modules of wrong type from buttons module list.
        * See populateMenu() and BTModuleTreeItem. */
        struct TypeFilter : public BTModuleTreeItem::Filter {
            TypeFilter(CSwordModuleInfo::ModuleType t) {
                m_mType = t;
            }
            bool filter(CSwordModuleInfo* mi) {
                return ((mi->type() == m_mType) && !mi->isLocked());
            }
            CSwordModuleInfo::ModuleType m_mType;
        };

        /**
        * A new empty widget. updateMenu() is needed to update the label, menu items etc.
        */
        BtTextWindowHeaderWidget(BtTextWindowHeader *parent, CSwordModuleInfo::ModuleType mtype);
        /** Does nothing. All resources are freed and children deleted automatically.*/
        ~BtTextWindowHeaderWidget();

    public:
        /**
        * Called after the window module list has changed. Updates the module name and
        * the existing menu items but doesn't add or remove them if the menu exists.
        * If the menu doesn't exist, creates it first and then updates it.
        */
        void updateWidget(QStringList newModulesToUse, QString thisModule, int newIndex);

        /** Creates the menu from scratch and updates the items using updateMenu().*/
        void recreateWidget(QStringList newModulesToUse, QString thisModule, int newIndex);

    signals:
        /** User selected a module from menu to replace an existing module.*/
        void sigModuleReplace ( int index, QString newModule );
        /** User selected a module from menu to add. */
        void sigModuleAdd ( int index, QString module );
        /** User selected a module from menu to be removed. */
        void sigModuleRemove ( int index );

    private slots:
        /** Handle the action signal from the menu.*/
        void moduleChosen(QAction* action );

    private:

        /**
        * Populates the menu with language submenus and module items without setting
        * their states.
        */
        void populateMenu();
        /** Adds items to the menu recursively. */
        void addItemToMenu(BTModuleTreeItem* item, QMenu* menu, TypeOfAction actionType);

    private:

        int m_id;
        QAction* m_removeAction;
        CSwordModuleInfo::ModuleType m_moduleType;
        QString m_module;
        QLabel* m_label;
        QToolButton* m_button;
        QFrame* m_separator;
        QMenu* m_popup;
        QList<QMenu*> m_submenus;
};

#endif
