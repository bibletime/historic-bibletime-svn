/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "bttextwindowheader.h"

#include "bttextwindowheaderwidget.h"
#include "clexiconreadwindow.h"

#include <QStringList>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QToolButton>
#include <QFrame>
#include <QAction>
#include <QDebug>

BtTextWindowHeader::BtTextWindowHeader ( CDisplayWindow* window, CSwordModuleInfo::ModuleType modtype, QStringList modules )
        : QWidget ( window ),
        BtWindowModuleChooser(window, modtype) {
    QHBoxLayout* layout = new QHBoxLayout ( this );
    layout->setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setLayoutDirection(Qt::LeftToRight);
    setModules(modules);
    connect(window, SIGNAL(sigModuleListSet(QStringList)), SLOT(slotBackendModulesChanged()));
    connect(window, SIGNAL(sigModuleListChanged()), SLOT(slotWindowModulesChanged()));
}

BtTextWindowHeader::~BtTextWindowHeader() {}

void BtTextWindowHeader::slotBackendModulesChanged() {
    m_modules = m_window->getModuleList();

    adjustWidgetCount();

    //recreate all widgets from scratch
    for (int i = 0; i < m_widgetList.count(); i++) {
        BtTextWindowHeaderWidget* widgt = m_widgetList.at(i);
        QString moduleName = m_modules.at(i);
        qDebug() << "refresh button's menu:" << moduleName << i;
        widgt->recreateWidget(m_modules, moduleName, i);
    }
}

void BtTextWindowHeader::slotWindowModulesChanged() {
    m_modules = m_window->getModuleList();
    adjustWidgetCount();
    updateWidgets();
}

void BtTextWindowHeader::adjustWidgetCount(bool adjustToZero) {
    //qDebug() << "BtModuleChooserBar::ajustButtonCount";
    int widgetCountDifference = 0;
    if (adjustToZero) {
        widgetCountDifference = m_widgetList.count();
    }
    else {
        widgetCountDifference = m_widgetList.count() - (m_modules.count());
    }
    if (m_moduleType == CSwordModuleInfo::GenericBook && !adjustToZero) {
        widgetCountDifference = (1 - m_widgetList.count()) * -1;
    }
    //if there are more buttons than modules, delete buttons
    if (widgetCountDifference > 0) {
        while (widgetCountDifference) {
            // it should be safe to delete the button later
            BtTextWindowHeaderWidget* w = m_widgetList.takeFirst();
            w->setParent(0);
            w->deleteLater();
            widgetCountDifference--;
        }
    }
    // if there are more modules than buttons, add buttons
    if (widgetCountDifference < 0) {
        while (widgetCountDifference) {
            addWidget();
            widgetCountDifference++;
        }
    }
}

BtTextWindowHeaderWidget* BtTextWindowHeader::addWidget() {
    BtTextWindowHeaderWidget* w = new BtTextWindowHeaderWidget(this, m_moduleType);
    layout()->addWidget(w);
    m_widgetList.append(w);

    // the button sends signals directly to the window which then signals back when the module
    // list has changed
    connect(w, SIGNAL(sigModuleAdd(int, QString)), m_window, SLOT(slotAddModule(int, QString)));
    connect(w, SIGNAL(sigModuleReplace(int, QString)), m_window, SLOT(slotReplaceModule(int, QString)));
    connect(w, SIGNAL(sigModuleRemove(int)), m_window, SLOT(slotRemoveModule(int)));

    return w;
}

void BtTextWindowHeader::setModules( QStringList useModules ) {
    qDebug() << "BtModuleChooserBar::setModules";
    m_modules = useModules;
    adjustWidgetCount(true);

    //if (!useModules.count()) return;
    for (int i = 0; i < useModules.count(); i++) {
        addWidget();
    }
    updateWidgets();
    //qDebug() << "BtModuleChooserBar::setModules end";
}

void BtTextWindowHeader::updateWidgets() {
    for (int i = 0; i < m_widgetList.count(); i++) {
        BtTextWindowHeaderWidget* w = m_widgetList.at(i);
        //QString moduleName = m_modules.at(i);
        //qDebug() << "refresh button's menu:" << moduleName << i;
        w->updateWidget(m_modules, m_modules.at(i), i);
    }
}
