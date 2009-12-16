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

#include "backend/bookshelfmodel/item.h"

#include "backend/bookshelfmodel/btbookshelfmodel.h"


namespace BookshelfModel {

Item::Item(Type type)
        : m_type(type), m_parent(0), m_checkState(Qt::Unchecked) {
    // Intentionally empty
}

Item::~Item() {
    qDeleteAll(m_children);
}

int Item::indexFor(Item *newItem) {
    Q_ASSERT(newItem != 0);

    if (m_children.empty()) return 0;

    int i(0);
    for (;;) {
        Item *nextItem(m_children.at(i));
        Q_ASSERT(nextItem->type() == newItem->type());
        if (*newItem < *nextItem) {
            return i;
        }
        i++;
        if (i >= m_children.size()) {
            return i;
        }
    }
}

QVariant Item::data(int role) const {
    switch (role) {
        case Qt::CheckStateRole:
            return m_checkState;
        case BtBookshelfModel::ModuleHiddenRole:
            if (m_children.empty()) return true;

            foreach (Item *child, m_children) {
                if (!child->data(role).toBool()) return false;
            }
            return true;
        default:
            return QVariant();
    }
}

bool Item::operator<(const Item &other) const {
    if (m_type != other.type()) {
        return m_type < other.type();
    }
    QString first(data(Qt::DisplayRole).toString().toLower());
    QString second(other.data(Qt::DisplayRole).toString().toLower());
    return first.localeAwareCompare(second) < 0;
}

} // namespace BookshelfModel
