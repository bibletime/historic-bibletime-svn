/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BTBOOKMARKITEM_H
#define BTBOOKMARKITEM_H

#include "frontend/mainindex/bookmarks/btbookmarkitembase.h"

#include <QString>


class BtBookmarkFolder;
class CSwordModuleInfo;

class BtBookmarkItem : public BtBookmarkItemBase {
    public:
        friend class BtBookmarkLoader;

        BtBookmarkItem(QTreeWidgetItem* parent);

        /** Creates a bookmark with module, key and description. */
        BtBookmarkItem(const CSwordModuleInfo *module, const QString &key,
                       const QString &description, const QString &title);

        /** Creates a copy. */
        BtBookmarkItem(const BtBookmarkItem& other);

        ~BtBookmarkItem() {}

        /** Returns the used module, 0 if there is no such module. */
        CSwordModuleInfo *module() const;

        /** Returns the used key. */
        QString key() const;

        /** Returns the used description. */
        inline const QString &description() const {
            return m_description;
        }

        /** Returns the title. */
        inline const QString &title() const {
            return m_title;
        }

        /** Returns a tooltip for this bookmark. */
        virtual QString toolTip() const;

        /** Returns whether the action is supported by this item. */
        virtual bool enableAction(MenuAction action);

        /** Changes this bookmark. */
        virtual void rename();

        void update();

    private:
        /** Returns the english key.*/
        inline const QString &englishKey() const {
            return m_key;
        }

    private:
        QString m_key;
        QString m_description;
        QString m_moduleName;
        QString m_title;
};

#endif
