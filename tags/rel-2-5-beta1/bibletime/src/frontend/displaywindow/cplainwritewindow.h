/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CPLAINWRITEWINDOW_H
#define CPLAINWRITEWINDOW_H

#include "frontend/displaywindow/cwritewindow.h"


class BtActionCollection;
class QAction;
class QString;

/** The write window class which offers a plain editor for source code editing.
  * @author The BibleTime team
  */
class CPlainWriteWindow : public CWriteWindow  {
        Q_OBJECT
    public:
        CPlainWriteWindow( QList<CSwordModuleInfo*> modules, CMDIArea* parent);
        virtual ~CPlainWriteWindow();

        /**
        * Store the settings of this window in the given CProfileWindow object.
        */
        virtual void storeProfileSettings( Profile::CProfileWindow* profileWindow );
        /**
        * Store the settings of this window in the given profile window.
        */
        virtual void applyProfileSettings( Profile::CProfileWindow* profileWindow );

        /**
         * Setups the popup menu of this display widget.
         */
        virtual void setupPopupMenu();
        virtual bool syncAllowed() const;

    protected: // Protected methods
        /**
        * Initialize the state of this widget.
        */
        virtual void initView();
        virtual void initConnections();
        virtual void initToolbars();
        virtual CDisplayWindow::WriteWindowType writeWindowType() {
            return CDisplayWindow::PlainTextWindow;
        };

        /**
         * Initializes the intern keyboard actions.
         */
        virtual void initActions();
        /**
        * Insert the keyboard accelerators of this window into the given KAccel object.
        */
        static void insertKeyboardActions( BtActionCollection* const a );

    private:
        struct {
            QAction* saveText;
            QAction* deleteEntry;
            QAction* restoreText;
            QAction* syncWindow;
        }
        m_actions;

    protected slots: // Protected slots
        /**
        * Saves the text for the current key. Directly writes the changed text into the module.
        */
        virtual void saveCurrentText( const QString& );
        /**
        * Is called when the current text was changed.
        */
        virtual void textChanged();
        /**
        * Loads the original text from the module.
        */
        virtual void restoreText();
        /**
        * Deletes the module entry and clears the edit widget.
        */
        virtual void deleteEntry();
};

#endif
