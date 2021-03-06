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

#ifndef BTCHECKSTATEFILTERPROXYMODEL_H
#define BTCHECKSTATEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class BtCheckStateFilterProxyModel: public QSortFilterProxyModel {
        Q_OBJECT
    public:
        BtCheckStateFilterProxyModel(QObject *parent = 0);
        virtual ~BtCheckStateFilterProxyModel();

        inline bool enabled() const {
            return m_enabled;
        }
        void setEnabled(bool enable);

        inline bool showChecked() const {
            return m_showChecked;
        }
        void setShowChecked(bool show);

        inline bool showUnchecked() const {
            return m_showUnchecked;
        }
        void setShowUnchecked(bool show);

        inline bool showPartiallyChecked() const {
            return m_showPartiallyChecked;
        }
        void setShowPartiallyChecked(bool show);

        virtual bool filterAcceptsRow(int row, const QModelIndex &parent) const;

    protected:
        bool m_enabled;
        bool m_showChecked;
        bool m_showUnchecked;
        bool m_showPartiallyChecked;
};

#endif // BTSELECTEDMODULESBOOKSHELFPROXYMODEL_H
