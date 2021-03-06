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

#ifndef DISTRIBUTIONITEM_H
#define DISTRIBUTIONITEM_H

#include "backend/bookshelfmodel/item.h"

#include "backend/drivers/cswordmoduleinfo.h"

namespace BookshelfModel {

class DistributionItem: public Item {
    public:
        static const Item::Type GROUP_TYPE = Item::ITEM_DISTRIBUTION;

        DistributionItem(CSwordModuleInfo *module);

        inline QString distribution() const { return m_distribution; }

        inline QString name() const { return m_distribution; }

        inline bool fitFor(CSwordModuleInfo *module) {
            return module->config(CSwordModuleInfo::DistributionSource)
                       == m_distribution;
        }

    protected:
        QString m_distribution;
};

} // namespace BookshelfModel

#endif // DISTRIBUTIONITEM_H
