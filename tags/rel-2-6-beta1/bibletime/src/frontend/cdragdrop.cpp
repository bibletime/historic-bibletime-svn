/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "frontend/cdragdrop.h"

#include <QByteArray>
#include <QList>
#include <QMimeData>
#include <QString>

BTMimeData::BTMimeData() {}

BTMimeData::~BTMimeData() {}

/** Creates a new BTMimeData.
* Creates a new bookmark item and appends it to the list.
*/
BTMimeData::BTMimeData(QString module, QString key, QString description) {
    appendBookmark(module, key, description);
}

/** Creates a new BTMimeData, setting the text MIME type (see QMimeData::setText()). */
BTMimeData::BTMimeData(QString text) {
    setText(text);
}

/** Appends a new bookmark item into the list.*/
void BTMimeData::appendBookmark(QString module, QString key, QString description) {
    BookmarkItem bm = BookmarkItem(module, key, description);
    m_bookmarkList.append(bm);
    setData("BibleTime/Bookmark", QByteArray());
}



/** Returns the first bookmark item in the list. */
const BookmarkItem& BTMimeData::bookmark() const {
    return m_bookmarkList.first();
}

BookmarkItem::BookmarkItem(const QString &module, const QString &key,
                           const QString &description)
        : m_moduleName(module), m_key(key), m_description(description)
{
    // Intentionally empty
}
