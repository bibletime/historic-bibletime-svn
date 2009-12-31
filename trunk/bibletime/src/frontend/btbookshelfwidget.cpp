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

#include "frontend/btbookshelfwidget.h"

#include <QActionGroup>
#include <QApplication>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QToolButton>
#include <QVBoxLayout>
#include "backend/bookshelfmodel/btbookshelffiltermodel.h"
#include "frontend/bookshelfmanager/installpage/btinstallmodulechooserdialogmodel.h"
#include "frontend/bookshelfmanager/removepage/btremovepagetreemodel.h"
#include "frontend/btbookshelfdockwidget.h"
#include "frontend/btbookshelfview.h"
#include "util/cresmgr.h"
#include "util/directory.h"


BtBookshelfWidget::BtBookshelfWidget(QWidget *parent, Qt::WindowFlags flags)
        : QWidget(parent, flags), m_sourceModel(0), m_treeModel(0)
{
    // Setup post-filter:
    m_postFilterModel = new BtBookshelfFilterModel(this);

    // Init widgets and such:
    initActions();
    initMenus();
    initWidgets();

    // Connect treeview to model:
    m_treeView->setModel(m_postFilterModel);

    retranslateUi();

    connect(m_nameFilterEdit,  SIGNAL(textEdited(QString)),
            m_postFilterModel, SLOT(setNameFilterFixedString(QString)));
    connect(m_treeView, SIGNAL(contextMenuActivated(QPoint)),
            this,       SLOT(slotShowContextMenu(QPoint)));
    connect(m_treeView, SIGNAL(moduleContextMenuActivated(CSwordModuleInfo*, QPoint)),
            this,       SLOT(slotShowItemContextMenu(CSwordModuleInfo*, QPoint)));
}

BtBookshelfWidget::~BtBookshelfWidget() {
    // Intentionally empty
}

void BtBookshelfWidget::setSourceModel(QAbstractItemModel *model) {
    Q_ASSERT(model != 0);
    m_sourceModel = model;
    if (m_treeModel != 0) {
        m_treeModel->setSourceModel(model);
    }
}

void BtBookshelfWidget::setTreeModel(BtBookshelfTreeModel *model) {
    Q_ASSERT(model != 0);
    Q_ASSERT(m_treeModel == 0);
    m_treeModel = model;
    if (m_sourceModel != 0) {
        model->setSourceModel(m_sourceModel);
    }
    m_postFilterModel->setSourceModel(model);
}

void BtBookshelfWidget::initActions() {
    namespace DU = util::directory;
    namespace RM = CResMgr::mainIndex;

    // Grouping action group and actions:
    m_groupingActionGroup = new QActionGroup(this);
    m_groupingActionGroup->setExclusive(true);
    connect(m_groupingActionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(slotGroupingActionTriggered(QAction*)));

    m_groupingBookshelfAction = new QAction(this);
    m_groupingBookshelfAction->setCheckable(true);
    m_groupingBookshelfAction->setChecked(true);
    m_groupingActionGroup->addAction(m_groupingBookshelfAction);

    m_groupingCatLangAction = new QAction(this);
    m_groupingCatLangAction->setCheckable(true);
    m_groupingActionGroup->addAction(m_groupingCatLangAction);

    m_groupingCatAction = new QAction(this);
    m_groupingCatAction->setCheckable(true);
    m_groupingActionGroup->addAction(m_groupingCatAction);

    m_groupingLangCatAction = new QAction(this);
    m_groupingLangCatAction->setCheckable(true);
    m_groupingActionGroup->addAction(m_groupingLangCatAction);

    m_groupingLangAction = new QAction(this);
    m_groupingLangAction->setCheckable(true);
    m_groupingActionGroup->addAction(m_groupingLangAction);

    m_groupingNoneAction = new QAction(this);
    m_groupingNoneAction->setCheckable(true);
    m_groupingActionGroup->addAction(m_groupingNoneAction);

    m_showHideAction = new QAction(this);
    m_showHideAction->setIcon(DU::getIcon("layer-visible-on.svg"));
    m_showHideAction->setCheckable(true);
    connect(m_showHideAction, SIGNAL(toggled(bool)),
            m_postFilterModel, SLOT(setShowHidden(bool)));
}

void BtBookshelfWidget::initMenus() {
    namespace DU = util::directory;
    namespace RM = CResMgr::mainIndex;

    // Grouping menu:
    m_groupingMenu = new QMenu(this);
    m_groupingMenu->setIcon(DU::getIcon(RM::grouping::icon));
    m_groupingMenu->addAction(m_groupingBookshelfAction);
    m_groupingMenu->addSeparator();
    m_groupingMenu->addAction(m_groupingCatLangAction);
    m_groupingMenu->addAction(m_groupingCatAction);
    m_groupingMenu->addAction(m_groupingLangCatAction);
    m_groupingMenu->addAction(m_groupingLangAction);
    m_groupingMenu->addAction(m_groupingNoneAction);

    // Context menu
    m_contextMenu = new QMenu(this);
    m_contextMenu->addMenu(m_groupingMenu);
    m_contextMenu->addAction(m_showHideAction);

    // Item context menu
    m_itemContextMenu = m_contextMenu;
}

void BtBookshelfWidget::initWidgets() {
    QVBoxLayout *layout(new QVBoxLayout);
    layout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *toolBar(new QHBoxLayout);
    // Add a small margin between the edge of the window and the label (looks better)
    toolBar->setContentsMargins(3, 0, 0, 0);
    m_nameFilterLabel = new QLabel(this);
    toolBar->addWidget(m_nameFilterLabel);

    m_nameFilterEdit = new QLineEdit(this);
    m_nameFilterEdit->installEventFilter(this);
    m_nameFilterLabel->setBuddy(m_nameFilterEdit);
    toolBar->addWidget(m_nameFilterEdit);

    m_groupingButton = new QToolButton(this);
    m_groupingButton->setPopupMode(QToolButton::InstantPopup);
    m_groupingButton->setMenu(m_groupingMenu);
    m_groupingButton->setIcon(m_groupingMenu->icon());
    m_groupingButton->setAutoRaise(true);
    toolBar->addWidget(m_groupingButton);

    m_showHideButton = new QToolButton(this);
    m_showHideButton->setDefaultAction(m_showHideAction);
    m_showHideButton->setAutoRaise(true);
    toolBar->addWidget(m_showHideButton);
    layout->addLayout(toolBar);

    m_treeView = new BtBookshelfView(this);
    layout->addWidget(m_treeView);
    setLayout(layout);
}

void BtBookshelfWidget::retranslateUi() {
    m_nameFilterLabel->setText(tr("Fi&lter:"));
    m_groupingButton->setText(tr("Grouping"));
    m_groupingButton->setToolTip(tr("Change the grouping of items in the bookshelf."));

    m_groupingMenu->setTitle(tr("Grouping"));
    m_groupingBookshelfAction->setText(tr("Use grouping from bookshelf dock"));
    m_groupingCatLangAction->setText(tr("Category/Language"));
    m_groupingCatAction->setText(tr("Category"));
    m_groupingLangCatAction->setText(tr("Language/Category"));
    m_groupingLangAction->setText(tr("Language"));
    m_groupingNoneAction->setText(tr("No grouping"));
    m_showHideAction->setText(tr("Show/hide works"));
}

bool BtBookshelfWidget::eventFilter(QObject *object, QEvent *event) {
    Q_ASSERT(object == m_nameFilterEdit);
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        switch (e->key()) {
            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Enter:
            case Qt::Key_Return:
                QApplication::sendEvent(m_treeView, event);
                return true;
            default:
                break;
        }
    }
    return false;
}

void BtBookshelfWidget::slotGroupingActionTriggered(QAction *action) {
    BtBookshelfDockWidget *dockWidget(BtBookshelfDockWidget::getInstance());

    if (action == m_groupingBookshelfAction) {
        connect(dockWidget, SIGNAL(groupingOrderChanged(BtBookshelfTreeModel::Grouping)),
                m_treeModel, SLOT(setGroupingOrder(const BtBookshelfTreeModel::Grouping &)));
        m_treeModel->setGroupingOrder(dockWidget->groupingOrder());
        m_treeView->setRootIsDecorated(!dockWidget->groupingOrder().isEmpty());
        return;
    }
    disconnect(dockWidget, SIGNAL(groupingOrderChanged(BtBookshelfTreeModel::Grouping)),
               m_treeModel, SLOT(setGroupingOrder(const BtBookshelfTreeModel::Grouping &)));

    BtBookshelfTreeModel::Grouping grouping;
    if (action == m_groupingCatAction) {
        grouping.append(BtBookshelfTreeModel::GROUP_CATEGORY);
    }
    else if (action == m_groupingCatLangAction) {
        grouping.append(BtBookshelfTreeModel::GROUP_CATEGORY);
        grouping.append(BtBookshelfTreeModel::GROUP_LANGUAGE);
    }
    else if (action == m_groupingLangAction) {
        grouping.append(BtBookshelfTreeModel::GROUP_LANGUAGE);
    }
    else if (action == m_groupingLangCatAction) {
        grouping.append(BtBookshelfTreeModel::GROUP_LANGUAGE);
        grouping.append(BtBookshelfTreeModel::GROUP_CATEGORY);
    }
    m_treeModel->setGroupingOrder(grouping);
    m_treeView->setRootIsDecorated(!grouping.isEmpty());
}

void BtBookshelfWidget::slotShowContextMenu(const QPoint &pos) {
    m_contextMenu->popup(pos);
}

void BtBookshelfWidget::slotShowItemContextMenu(CSwordModuleInfo *module, const QPoint &pos)
{
    if (m_itemContextMenu != 0) {
        m_itemContextMenu->setProperty("BtModule", qVariantFromValue((void*) module));
        m_itemContextMenu->popup(pos);
    } else {
        m_itemContextMenu = m_contextMenu;
        slotShowItemContextMenu(module, pos);
    }
}
