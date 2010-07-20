/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CSEARCHDIALOG_H
#define CSEARCHDIALOG_H

#include <QDialog>

#include <QString>
#include "backend/cswordmodulesearch.h"
#include "backend/managers/cswordbackend.h"
#include "frontend/searchdialog/btsearchoptionsarea.h"

namespace Search {
class BtSearchResultArea;
}
class QPushButton;
class QWidget;

namespace Search {

/**
  \note destroys itself on close
*/
class CSearchDialog : public QDialog {
        Q_OBJECT
    public:
        static void openDialog(const QList<const CSwordModuleInfo*> modules,
                               const QString &searchText = QString::null,
                               QWidget *parentDialog = 0);

        static void closeDialog();

    protected:
        friend class CSearchAnalysisScene;
        friend class CSearchResultArea;
        friend class BtSearchResultArea;
        friend class BibleTime;

        /**
        * Only interesting for the class members! Useful to get the searched text etc.
        */
        static CSearchDialog* getSearchDialog();

        CSearchDialog(QWidget *parent);
        ~CSearchDialog();

        /**
         * Initializes this object.
         */
        void initView();

        /**
          Starts the search with the given module list and given search text.
        */
        void startSearch(const QList<const CSwordModuleInfo*> modules,
                         const QString &searchText);

        /**Prepares the search string given by user for a specific search type */
        QString prepareSearchText(const QString& orig);

        /**
          Sets the list of modules for the search.
        */
        void setModules(const QList<const CSwordModuleInfo*> modules) {
            m_searchOptionsArea->setModules(modules);
        }

        /**
          Returns the list of used modules.
        */
        inline QList<const CSwordModuleInfo*> modules() const {
            return m_searchOptionsArea->modules();
        }

        /**
        * Sets the search text which is used for the search.
        */
        void setSearchText( const QString &searchText );

        /**
          \returns the search text which is set currently.
        */
        QString searchText() const {
            return m_searchOptionsArea->searchText();
        }

        /**
          \returns the used search scope as a list key
        */
        inline sword::ListKey searchScope() const {
            return m_searchOptionsArea->searchScope();
        }

        /**
        * Resets the parts to the default.
        */
        void reset();
        /**
        * Load the settings from the resource file
        */
        void loadDialogSettings();
        /**
        * Save the settings to the resource file
        */
        void saveDialogSettings();

    protected slots:
        /**
          Starts the search with the set modules and the set search text.
        */
        void startSearch();

        void showModulesSelector();
        /**
        * Initializes the signal slot connections
        */
        void initConnections();

        void closeButtonClicked();

    private:
        QPushButton* m_analyseButton;
        QPushButton* m_closeButton;
        BtSearchResultArea* m_searchResultArea;
        BtSearchOptionsArea* m_searchOptionsArea;

        CSwordModuleSearch m_searcher;
};


} //end of namespace Search

#endif
