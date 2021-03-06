/*********
*
* In the name of the Father, and of the Son, and of the Holy Spirit.
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License
* version 2.0.
*
**********/

#ifndef BTOPENWORKACTION_H
#define BTOPENWORKACTION_H

#include <QAction>
#include "frontend/btmenuview.h"

#include "backend/bookshelfmodel/btbookshelftreemodel.h"


class BtBookshelfTreeModel;
class BtBookshelfFilterModel;
class CSwordModuleInfo;

class BtOpenWorkActionMenu: public BtMenuView {
    Q_OBJECT
    public:
        BtOpenWorkActionMenu(QWidget *parent = 0);
        ~BtOpenWorkActionMenu();

        void setSourceModel(QAbstractItemModel *model);
        inline QAbstractItemModel *sourceModel() const { return m_treeModel->sourceModel(); }
        inline BtBookshelfTreeModel *treeModel() const { return m_treeModel; }
        inline BtBookshelfFilterModel *postFilterModel() const { return m_postFilterModel; }

    signals:
        void triggered(CSwordModuleInfo *module);

    private slots:
        void slotIndexTriggered(const QModelIndex &index);

    private:
        // Models:
        BtBookshelfTreeModel   *m_treeModel;
        BtBookshelfFilterModel *m_postFilterModel;
};

class BtOpenWorkAction: public QAction {
    Q_OBJECT
    public:
        explicit BtOpenWorkAction(QObject *parent = 0);
        ~BtOpenWorkAction();

    signals:
        void triggered(CSwordModuleInfo *module);

    protected:
        void retranslateUi();

    private slots:
        void slotModelChanged();

    private:
        BtOpenWorkActionMenu *m_menu;
};

#endif // BTOPENWORKACTION_H
