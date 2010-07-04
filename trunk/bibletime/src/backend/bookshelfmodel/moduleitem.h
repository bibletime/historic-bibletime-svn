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

#ifndef MODULEITEM_H
#define MODULEITEM_H

#include "backend/bookshelfmodel/item.h"

#include "backend/bookshelfmodel/btbookshelfmodel.h"
#include "backend/drivers/cswordmoduleinfo.h"


class BtBookshelfTreeModel;

namespace BookshelfModel {

class ModuleItem: public Item {
    public:
        ModuleItem(CSwordModuleInfo *module, BtBookshelfTreeModel *parentModel);

        /**
          Reimplementation of Item::data() which dispatches all
          requests to the parent model (BtBookshelfTreeModel).
        */
        virtual QVariant data(int role = Qt::DisplayRole) const;

        inline CSwordModuleInfo *moduleInfo() const {
            return m_moduleInfo;
        }

    protected:
        CSwordModuleInfo     *m_moduleInfo;
        BtBookshelfTreeModel *m_parentModel;
};

} // namespace BookshelfModel

#endif // MODULEITEM_H
