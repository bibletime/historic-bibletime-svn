/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BTDISPLAYSETTINGSBUTTON_H
#define BTDISPLAYSETTINGSBUTTON_H

#include <QToolButton>

#include "backend/managers/cswordbackend.h"
#include "btglobal.h"


class CSwordModuleInfo;
class QMenu;

/** This class manages the display options of the selected modules.
  * @author The BibleTime team
  */
class BtDisplaySettingsButton: public QToolButton {
        Q_OBJECT

    public:
        BtDisplaySettingsButton(QWidget *parent = 0);

    public slots:
        void setDisplayOptions(const DisplayOptions &displaySettings,
                               bool repopulate = true);
        void setFilterOptions(const FilterOptions &moduleSettings,
                              bool repopulate = true);

        void setModules(const QList<const CSwordModuleInfo*> &modules);

    signals:
        void sigFilterOptionsChanged(FilterOptions filterOptions);
        void sigDisplayOptionsChanged(DisplayOptions displayOptions);
        void sigModulesChanged(const QList<CSwordModuleInfo*> &modules);
        void sigChanged(void);

    protected slots:
        void slotOptionToggled(QAction *action);

    protected:
        void initMenu();
        void retranslateUi();
        void retranslateToolTip();
        void repopulateMenu();

        bool isOptionAvailable(const CSwordModuleInfo::FilterTypes option);
        void addMenuEntry(QAction *action, bool checked);

    private:
        FilterOptions  m_filterOptions;
        DisplayOptions m_displayOptions;
        QList<const CSwordModuleInfo*> m_modules;

        QMenu *m_popup;
        QAction *m_lineBreakAction;
        QAction *m_verseNumbersAction;
        QAction *m_headingsAction;
        QAction *m_redWordsAction;
        QAction *m_hebrewPointsAction;
        QAction *m_hebrewCantillationAction;
        QAction *m_greekAccentsAction;
        QAction *m_variantAction;
        QAction *m_scriptureReferencesAction;
        QAction *m_morphSegmentationAction;
};

#endif
