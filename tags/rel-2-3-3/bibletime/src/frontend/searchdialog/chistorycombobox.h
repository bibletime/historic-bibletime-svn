/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CHISTORYCOMBOBOX_H
#define CHISTORYCOMBOBOX_H

#include <QComboBox>

namespace Search {

class CHistoryComboBox : public QComboBox {
        Q_OBJECT
    public:
        CHistoryComboBox(QWidget* parent = 0);
        ~CHistoryComboBox();
        void addToHistory(const QString& item);
        QStringList historyItems() const;

    protected:

    private:
};

} //end of namespace Search

#endif
