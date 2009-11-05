/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2009 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "frontend/keychooser/bthistory.h"

#include <QAction>
#include <QDebug>
#include <QList>
#include "backend/keys/cswordkey.h"


BTHistory::BTHistory(QWidget* parent)
        : m_historyList(),
        m_index(-1),
        m_inHistoryFunction(false) {
    setParent(parent);
    Q_ASSERT(class_invariant());
}

void BTHistory::add(CSwordKey* newKey) {
    qDebug() << "BTHistory::add";
    Q_ASSERT(newKey);
    // Add new key Action after current index if we were not using the history functions,
    // if it's not a duplicate and if it's not empty.
    if (!m_inHistoryFunction &&	((m_index < 0) || (newKey->key() != m_historyList.at(m_index)->text()) )) {
        if (!newKey->key().isEmpty()) {
            m_historyList.insert(++m_index, new QAction(newKey->key(), this));
        }
        // \todo history limit?
        sendChangedSignal();
    }
    Q_ASSERT(class_invariant());
}

void BTHistory::move(QAction* historyItem) {
    qDebug() << "BTHistory::move";
    //Q_ASSERT(historyItem);
    Q_ASSERT(m_historyList.count());

    m_inHistoryFunction = true;
    //find the action in the list
    m_index = m_historyList.indexOf(historyItem);
    //move to the selected item in the list, it will be the current item
    QString newKey = m_historyList.at(m_index)->text();
    emit historyMoved(newKey); // signal to "outsiders"; key has been changed
    sendChangedSignal();

    m_inHistoryFunction = false;
    Q_ASSERT(class_invariant());
}

void BTHistory::back() {
    qDebug() << "BTHistory::back";
    if ( m_index >= 1) {
        move(m_historyList.at(m_index - 1));
    }
    Q_ASSERT(class_invariant());
}

void BTHistory::fw() {
    qDebug() << "BTHistory::fw";
    if (m_index < (m_historyList.size() - 1)) {
        move(m_historyList.at(m_index + 1));
    }
    Q_ASSERT(class_invariant());
}

QList<QAction*> BTHistory::getBackList() {
    qDebug() << "BTHistory::getBackList";

    QList<QAction*> list;
    for (int i = m_index - 1; i >= 0; --i) {
        list.append(m_historyList.at(i));
    }

    qDebug() << "return:" << list;
    Q_ASSERT(class_invariant());
    return list;
}

QList<QAction*> BTHistory::getFwList() {
    qDebug() << "BTHistory::getFwList";

    QList<QAction*> list;
    //qDebug() << "historyList.size:" << m_historyList.size();
    for (int i = m_index + 1; i < m_historyList.size(); ++i) {
        //qDebug() << "i:" << i;
        list.append(m_historyList.at(i));
    }
    qDebug() << "return:" << list;

    Q_ASSERT(class_invariant());
    return list;
}

void BTHistory::sendChangedSignal() {
    bool backEnabled = m_index > 0; //there are items in the back list
    bool fwEnabled = m_historyList.size() > m_index + 1; //there are items in the fw list
    emit historyChanged(backEnabled, fwEnabled);
    Q_ASSERT(class_invariant());
}

bool BTHistory::class_invariant() {
    for (int i = 0; i < m_historyList.size(); ++i) {
        if (!m_historyList.at(i) || m_historyList.at(i)->text().isEmpty()) return false;
    }
    if (!(m_index >= -1 && m_index < m_historyList.size())) return false;
    return true;
}
