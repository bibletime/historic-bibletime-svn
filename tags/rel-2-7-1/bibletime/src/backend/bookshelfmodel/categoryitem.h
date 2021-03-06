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

#ifndef CATEGORYITEM_H
#define CATEGORYITEM_H

#include "backend/bookshelfmodel/item.h"

#include <QCoreApplication>
#include "backend/bookshelfmodel/btbookshelfmodel.h"
#include "backend/drivers/cswordmoduleinfo.h"


namespace BookshelfModel {

class CategoryItem: public Item {
        Q_DECLARE_TR_FUNCTIONS(CategoryItem);

    public:
        static const Item::Type GROUP_TYPE = Item::ITEM_CATEGORY;

        CategoryItem(CSwordModuleInfo *module);

        inline const CSwordModuleInfo::Category &category() const {
            return m_category;
        }

        QVariant data(int role = Qt::DisplayRole) const;

        inline bool fitFor(CSwordModuleInfo *module) const {
            return module->category() == m_category;
        }

        bool operator<(const Item &other) const;

    protected:
        CSwordModuleInfo::Category m_category;
};

} // namespace BookshelfModel

#endif // CATEGORYITEM_H
