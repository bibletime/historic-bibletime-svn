/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CPROFILE_H
#define CPROFILE_H

#include <QByteArray>
#include <QList>
#include "frontend/cmdiarea.h"
#include "frontend/profile/cprofilewindow.h"


namespace Profile {

/** Manages one profile file. Provides functions to save and restore settings of the available display windows.
  * @author The BibleTime team
  */
class CProfile {
    public:
        CProfile(const QString& fileName, const QString& name = QString::null);
        ~CProfile();

        /**
        * Saves the profile to the file given in the constructor.
        * @param windows The list of windows available in the profile.
        */
        bool save( QList<CProfileWindow*> windows );
        /**
        * Saves the profile to the file given in the constructor.
        */
        bool save();
        /**
        * Loads the profile from the file given in the constructor.
        * @return The list of profiled window which exist in the profile.
        */
        QList<CProfileWindow*> load();
        /**
        * Returns the name of this profile.
        */
        const QString& name();
        /**
        * Returns the filename used for this profile.
        */
        const QString& filename();
        /**
        * Initializes the XML for the first time (use to create a new profile)
        */
        void init(const QString);
        /**
        * Chnages the name of this profile.
        */
        void setName( const QString& );
        /**
        * Returns true if the main window was maximized as the profile was saved.
        */
        bool maximized() const;
        /**
        * Set the parameter to true if the main window is maximized.
        */
        void setMaximized( const bool maximized );
        /**
        * Sets the MDI arrangement mode
        */
        void setMDIArrangementMode(const CMDIArea::MDIArrangementMode);
        /**
        * Returns mdi arrangement mode
        */
        CMDIArea::MDIArrangementMode getMDIArrangementMode(void);
        /**
        * set mainwindow saveGeometry - size and position of window
        */
        void setMainwindowGeometry(const QByteArray& geometry);
        /**
         * Return mainwindow saveGeometry - size and position of window
        */
        QByteArray getMainwindowGeometry();
        /**
        * set mainwindow saveState - position of docking windows and toolbar
        */
        void setMainwindowState(const QByteArray& state);
        /**
         * Return mainwindow saveState - position of docking windows and toolbar
        */
        QByteArray getMainwindowState();

    private:
        /**
        * Loads the basic settings requires for proper operation.
        */
        void loadBasics();
        void saveBasics();

        QList<CProfileWindow*> m_profileWindows;
        QString m_name;
        QString m_filename;
        CMDIArea::MDIArrangementMode m_mdiArrangementMode;
        QByteArray m_mainwindowGeometry;
        QByteArray m_mainwindowState;
};

} //end of namespace Profile

#endif
