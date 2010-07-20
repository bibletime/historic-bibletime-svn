/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CSWORDMODULESEARCH_H
#define CSWORDMODULESEARCH_H

#include <QObject>

#include <QHash>

// Sword includes:
#include <listkey.h>


class CSwordModuleInfo;

/**
 * CSwordModuleSearch manages the search on Sword modules. It manages the thread(s)
 * and manages the different modules.
  *
  * @author The BibleTime team
  * @version $Id: cswordmodulesearch.h,v 1.34 2006/08/08 19:32:48 joachim Exp $
  */

class CSwordModuleSearch: public QObject {
        Q_OBJECT

    public: /* Types: */
        typedef QHash<const CSwordModuleInfo*, sword::ListKey> Results;

    public: /* Methods: */
        inline CSwordModuleSearch()
            : m_foundItems(0) {}

        /**
          Sets the text which should be search in the modules.
          \param[in] text the text to search.
        */
        inline void setSearchedText(const QString &text) {
            m_searchText = text;
        }

        /**
          Set the modules which should be searched.
          \param[in] modules the modules to search in.
        */
        inline void setModules(const QList<const CSwordModuleInfo*> &modules) {
            Q_ASSERT(!modules.empty());
            Q_ASSERT(unindexedModules(modules).empty());
            m_searchModules = modules;
        }

        /**
          Sets the search scope.
          \param[in] scope the scope used for the search.
        */
        void setSearchScope(const sword::ListKey &scope);

        /**
          Resets the search scope.
        */
        inline void resetSearchScope() {
            m_searchScope.ClearList();
        }

        /**
          \returns the search scope.
        */
        const sword::ListKey &searchScope() const {
            return m_searchScope;
        }

        /**
          Starts the search for the search text.
        */
        void startSearch();

        /**
          \returns the number of found items in the last search.
        */
        inline unsigned long foundItems() const {
            return m_foundItems;
        }

        /**
          \returns the results of the search.
        */
        const Results &results() const {
            return m_results;
        }

        /**
          \returns the list of unindexed modules in the given list.
        */
        static QList<const CSwordModuleInfo*> unindexedModules(
                const QList<const CSwordModuleInfo*> &modules);

    private: /* Fields: */
        QString                        m_searchText;
        sword::ListKey                 m_searchScope;
        QList<const CSwordModuleInfo*> m_searchModules;

        Results                        m_results;
        unsigned long                  m_foundItems;
};

#endif
