/*********
*
* In the name of the Father, and of the Son, and of the Holy Spirit.
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2009 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License
* version 2.0.
*
**********/

#ifndef BTMENUVIEW_H
#define BTMENUVIEW_H

#include <QMenu>

#include <QMap>
#include <QPersistentModelIndex>


class QAbstractItemModel;
class QActionGroup;

/**
  This is a special menu, which shows the contents of an item model. The menu is repopulated
  with data from the item model each time before its shown, and it does not in any other way
  react to changes in the model.

  The menu is built from items in the model which are below the given parent index. By
  default this parent index is invalid. When the menu is about to show, all items directly
  below the parent index are inserted. If a child item has children of its own it is inserted
  as a QMenu which will be built recursively. Otherwise the child item is inserted as a
  QAction which may be triggered by the user.

  When subclassing this menu, reimplement preBuildMenu() and postBuildMenu() to add new menu
  items before or after the menu is populated with data from the associated item model. You
  can also reimplement newAction() and newMenu() if you further want to tune the appearance
  or behaviour of this menu.

  \warning This menu might not properly handle changes in the source model while being shown,
           so beware to check whether the index emitted by triggered() is valid.
*/
class BtMenuView: public QMenu {
    Q_OBJECT
    public:
        BtMenuView(QWidget *parent = 0);
        ~BtMenuView();

        /**
          Sets or resets the data model for this menu and resets the parent index to an
          invalid index.
          \param[in] model Pointer to the data model to represent.
          \warning Do not (re)set the model when the menu is being shown!
        */
        void setModel(QAbstractItemModel *model);

        /**
          Returns a pointer to the data model associated with this menu.
          \retval 0 If this menu is not associated to any model.
        */
        inline QAbstractItemModel *model() const { return m_model; }

        /**
          Sets or resets the parent index for the items of the associated model, which to
          represent. The menu will only show data items below the given index. This function
          has no effect if the given index is valid, but does not belong to the model
          associated with this menu.
          \param[in] parentIndex the new parent index.
          \warning (Re)setting the parent index will only take effect the next time the menu
                   is to be shown.
          \warning Changing the model using setModel() will automatically reset this index.
        */
        void setParentIndex(const QModelIndex &parentIndex);

        /**
          Returns the parent index of the items of the assiciated model, which are to be
          represented by this menu. By default this is an invalid index.
        */
        QModelIndex parentIndex() const { return m_parentIndex; }

    signals:
        /**
          This signal is emitted when the user activates a menu item corresponding to an
          index in the associated model.
          \param index The index of the model which was activated.
        */
        void triggered(QModelIndex index);

    protected:
        /**
          This method is called by BtMenuView before populating itself with data from the
          model. Reimplement this method to add any menus/actions to this menu before the
          items of the menu. The default implementation does nothing.

          The model might be unset before this method is called. When the menu is about to be
          shown, this allows for this method to initialize the model on request. If the model
          is unset after this method returns, the menu is not populated with data from the
          item model.
        */
        virtual void preBuildMenu();

        /**
          This method is called by BtMenuView after populating itself with data from the
          model. If there was no model set, this method is still called after preBuildMenu().
          Reimplement this method to add any menus/actions to this menu after the items of
          the menu. The default implementation does nothing.
        */
        virtual void postBuildMenu();

        /**
          This method is called by BtMenuView to initialize an action to add to this menu. If
          the action corresponding to the given index is not to be added to this menu, please
          return 0.
          \param[in] parentMenu the parent menu under which the new action is to be added.
          \param[in] itemIndex the index of the item corresponding to the action.
        */
        virtual QAction *newAction(QMenu *parentMenu, const QModelIndex &itemIndex);

        /**
          This method is called by BtMenuView to initialize a menu to add to this menu. If
          the menu corresponding to the given index is not to be added to this menu, please
          return 0. If the menu should not be populated by BtMenuView itself, please use
          setProperty("BtMenuView_NoPopulate", true) on the menu to be returned by this
          method.
          \param[in] parentMenu the parent menu under which the new menu is to be added.
          \param[in] itemIndex the index of the item corresponding to the menu.
        */
        virtual QMenu *newMenu(QMenu *parentMenu, const QModelIndex &itemIndex);

    private:
        void buildMenu(QMenu *parentMenu, const QModelIndex &parentIndex);

    private slots:
        void slotAboutToShow();
        void slotAboutToHide();
        void slotActionTriggered(QAction *action);

    protected:
        QAbstractItemModel *m_model;
        QPersistentModelIndex m_parentIndex;
        QActionGroup *m_actions;
        QMap<QAction *, QPersistentModelIndex> m_indexMap;
};

#endif // BTMENUVIEW_H
