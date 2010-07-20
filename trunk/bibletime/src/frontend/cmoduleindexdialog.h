/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CMODULEINDEXDIALOG_H
#define CMODULEINDEXDIALOG_H

#include <QProgressDialog>

#include <QMutex>


class CSwordModuleInfo;

/**
  This dialog is used to index a list of modules and to show progress for that.
  While the indexing is in progress it creates a blocking, top level dialog which shows the progress
 * while the indexing is done.
*/
class CModuleIndexDialog: public QProgressDialog {
        Q_OBJECT
        Q_DISABLE_COPY(CModuleIndexDialog)

    public: /* Methods: */
        /**
          Creates and shows the indexing progress dialog and starts the actual
          indexing. It shows the dialog with progress information. In case
          indexing some module is unsuccessful or cancelled, any indices that
          were created for other given modules are deleted. After indexing, the
          dialog is closed.
          \param[in] modules The list of modules to index.
          \pre all given modules are unindexed
          \returns whether the indexing was finished successfully.
        */
        static bool indexAllModules(const QList<const CSwordModuleInfo*> &modules);

    private: /* Methods: */
        CModuleIndexDialog(int numModules);

        /**
          Shows the indexing progress dialog and starts the actual indexing. It
          shows the dialog with progress information. In case indexing some
          module is unsuccessful or cancelled, any indices that were created for
          other given modules are deleted. After indexing, the dialog is closed.
          \param[in] modules The list of modules to index.
          \pre all given modules are unindexed
          \returns whether the indexing was finished successfully.
        */
        bool indexAllModules2(const QList<const CSwordModuleInfo*> &modules);

    private slots:
        void slotModuleProgress(int percentage);
        void slotFinished();

    private: /* Fields: */
        static QMutex m_singleInstanceMutex;
        int m_currentModuleIndex;
};

#endif
