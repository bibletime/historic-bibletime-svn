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

#include "frontend/mainindex/btbookshelfview.h"

#include <QApplication>
#include <QHeaderView>
#include <QMouseEvent>
#include "backend/bookshelfmodel/btbookshelftreemodel.h"
#include "backend/drivers/cswordmoduleinfo.h"

BtBookshelfView::BtBookshelfView(QWidget *parent)
        : QTreeView(parent) {
    header()->hide();

    /*
      Uncommenting the following statement will hide the [+]expand/[-]collapse
      controls in front of first-level items, which might not be desirable since
      hiding them also means that one has to do a total of 2 clicks (double-
      click)to expand or collapse top-level items:
    */
    // setRootIsDecorated(false);

    connect(this, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotItemActivated(QModelIndex)));
}

BtBookshelfView::~BtBookshelfView() {
    // Intentionally empty
}

CSwordModuleInfo *BtBookshelfView::getModule(const QModelIndex &index) const {
    return (CSwordModuleInfo *) model()
           ->data(index, BtBookshelfModel::ModulePointerRole).value<void*>();
}

void BtBookshelfView::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Menu:
            scrollTo(currentIndex());
            {
                CSwordModuleInfo *i(getModule(currentIndex()));
                QRect itemRect(visualRect(currentIndex()));
                QPoint p(viewport()->mapToGlobal(itemRect.bottomLeft()));
                if (i == 0) {
                    emit contextMenuActivated(p);
                }
                else {
                    emit moduleContextMenuActivated(i, p);
                }
            }
            event->accept();
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter: {
            QModelIndex i(currentIndex());
            CSwordModuleInfo *m(getModule(i));
            if (m != 0) {
                emit moduleActivated(m);
            }
            else {
                setExpanded(i, !isExpanded(i));
            }
        }
        event->accept();
        break;
        default:
            QTreeView::keyPressEvent(event);
            break;
    }
}

void BtBookshelfView::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() == Qt::RightButton) {
        QModelIndex clickedItemIndex(indexAt(event->pos()));
        if (clickedItemIndex.isValid()) {
            setCurrentIndex(clickedItemIndex);
        }
        CSwordModuleInfo *i(getModule(clickedItemIndex));
        if (i == 0) {
            emit contextMenuActivated(mapToGlobal(event->pos()));
        }
        else {
            emit moduleContextMenuActivated(i, mapToGlobal(event->pos()));
        }
        event->accept();
    }
    else {
        QTreeView::mousePressEvent(event);
    }
}

void BtBookshelfView::slotItemActivated(const QModelIndex &index) {
    CSwordModuleInfo *i(getModule(index));
    if (i != 0) {
        emit moduleActivated(i);
    }
}
