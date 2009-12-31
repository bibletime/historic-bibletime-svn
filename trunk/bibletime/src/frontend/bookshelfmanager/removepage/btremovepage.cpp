/*********
*
* In the name of the Father, and of the Son, and of the Holy Spirit.
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License
* version 2.0.
*
**********/

#include "frontend/bookshelfmanager/removepage/btremovepage.h"

#include <QAction>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QToolButton>
#include "backend/bookshelfmodel/btbookshelffiltermodel.h"
#include "backend/managers/cswordbackend.h"
#include "frontend/btbookshelfdockwidget.h"
#include "frontend/btbookshelfview.h"
#include "frontend/btbookshelfwidget.h"
#include "util/cpointers.h"
#include "util/cresmgr.h"
#include "util/dialogutil.h"
#include "util/directory.h"

// Sword includes:
#include <swmgr.h>
#include <installmgr.h>


BtRemovePage::BtRemovePage()
        : BtConfigPage() {
    namespace DU = util::directory;

    QGridLayout* layout = new QGridLayout(this);
    layout->setMargin(5);

    layout->setSpacing(10);
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(2, 1);


    m_bookshelfWidget = new BtBookshelfWidget(this);
    m_bookshelfWidget->postFilterModel()->setShowHidden(true);
    const BtBookshelfDockWidget *dw(BtBookshelfDockWidget::getInstance());
    m_bookshelfWidget->setTreeModel(new BtRemovePageTreeModel(dw->groupingOrder(), this));
    m_bookshelfWidget->setSourceModel(CPointers::backend()->model());
    m_bookshelfWidget->showHideAction()->setVisible(false);
    m_bookshelfWidget->showHideButton()->hide();
    m_bookshelfWidget->treeView()->header()->show();
    m_bookshelfWidget->treeView()->header()->setResizeMode(QHeaderView::ResizeToContents);

    layout->addWidget(m_bookshelfWidget, 2, 0, 1, 2);

    m_removeButton = new QPushButton(tr("Remove..."), this);
    m_removeButton->setToolTip(tr("Remove the selected works"));
    m_removeButton->setIcon(DU::getIcon(CResMgr::bookshelfmgr::removepage::remove_icon));
    m_removeButton->setEnabled(false);
    layout->addWidget(m_removeButton, 3, 1, Qt::AlignRight);

    connect(m_removeButton, SIGNAL(clicked()),
            this, SLOT(slotRemoveModules()));
    connect(m_bookshelfWidget->treeModel(), SIGNAL(moduleChecked(CSwordModuleInfo*,bool)),
            this,                           SLOT(resetRemoveButton()));
    connect(m_bookshelfWidget->treeModel(), SIGNAL(rowsRemoved(const QModelIndex&,int,int)),
            this,                           SLOT(resetRemoveButton()));
}

QString BtRemovePage::label() {
    return tr("Remove installed works. Select the works and click Remove button.");
}

QString BtRemovePage::iconName() {
    return CResMgr::bookshelfmgr::removepage::icon;
}

QString BtRemovePage::header() {
    return tr("Remove");
}

void BtRemovePage::resetRemoveButton() {
    m_removeButton->setEnabled(!m_bookshelfWidget->treeModel()->checkedModules().empty());
}

void BtRemovePage::slotRemoveModules() {
    // Do nothing when this signal fires without anything selected to remove:
    if (m_bookshelfWidget->treeModel()->checkedModules().empty()) return;

    QStringList moduleNames;
    foreach (CSwordModuleInfo *m, m_bookshelfWidget->treeModel()->checkedModules()) {
        moduleNames.append(m->name());
    }
    const QString message = tr("You selected the following work(s): ")
                            .append(moduleNames.join(", "))
                            .append("\n\n")
                            .append(tr("Do you really want to remove them from your system?"));

    if ((util::showQuestion(this, tr("Remove Works?"), message, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)) {  //Yes was pressed.

        // Update the module list before really removing. Remember deleting the pointers later.
        QList<CSwordModuleInfo*> toBeDeleted = CPointers::backend()->takeModulesFromList(moduleNames);

        sword::InstallMgr installMgr;
        QMap<QString, sword::SWMgr*> mgrDict; //maps config paths to SWMgr objects
        foreach ( CSwordModuleInfo* mInfo, toBeDeleted ) {
            Q_ASSERT(mInfo); // Only installed modules could have been selected and returned by takeModulesFromList
            // Find the install path for the sword manager
            QString prefixPath = mInfo->config(CSwordModuleInfo::AbsoluteDataPath) + "/";
            QString dataPath = mInfo->config(CSwordModuleInfo::DataPath);
            if (dataPath.left(2) == "./") {
                dataPath = dataPath.mid(2);
            }
            if (prefixPath.contains(dataPath)) { //remove module part to get the prefix path
                prefixPath = prefixPath.remove( prefixPath.indexOf(dataPath), dataPath.length() );
            }
            else { //This is an error, should not happen
                qWarning() << "Removing" << mInfo->name() << "didn't succeed because the absolute path" << prefixPath << "didn't contain the data path" << dataPath;
                continue; // don't remove this, go to next of the for loop
            }

            // Create the sword manager and remove the module
            sword::SWMgr* mgr = mgrDict[ prefixPath ];
            if (!mgr) { //create new mgr if it's not yet available
                mgrDict.insert(prefixPath, new sword::SWMgr(prefixPath.toLocal8Bit()));
                mgr = mgrDict[ prefixPath ];
            }
            qDebug() << "Removing the module" << mInfo->name() << "...";
            installMgr.removeModule(mgr, mInfo->module()->Name());
        }
        //delete the removed moduleinfo pointers
        qDeleteAll(toBeDeleted);
        //delete all mgrs which were created above
        qDeleteAll(mgrDict);
        mgrDict.clear();
    }
}
