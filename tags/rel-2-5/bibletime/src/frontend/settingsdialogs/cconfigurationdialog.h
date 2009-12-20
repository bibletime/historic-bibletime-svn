/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2009 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CCONFIGURATIONDIALOG_H
#define CCONFIGURATIONDIALOG_H

#include "frontend/bookshelfmanager/btconfigdialog.h"


class BtActionCollection;
class CAcceleratorSettingsPage;
class CDisplaySettingsPage;
class CLanguageSettingsPage;
class CSwordSettingsPage;
class QAbstractButton;
class QDialogButtonBox;
class QWidget;

class CConfigurationDialog : public BtConfigDialog {
        Q_OBJECT
    public:
        CConfigurationDialog(QWidget *parent, BtActionCollection* actionCollection);
        virtual ~CConfigurationDialog();
        void save();

    protected slots:
        void slotButtonClicked(QAbstractButton *);

    private:
        BtActionCollection* m_actionCollection;
        CDisplaySettingsPage* m_displayPage;
        CSwordSettingsPage* m_swordPage;
        CAcceleratorSettingsPage* m_acceleratorsPage;
        CLanguageSettingsPage* m_languagesPage;
        QDialogButtonBox* m_bbox;

        // Load the settings from the resource file
        void loadDialogSettings();

        // Save the settings to the resource file
        void saveDialogSettings();

    signals:
        void signalSettingsChanged();
};


#endif
