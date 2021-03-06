/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



#include "chtmlwritewindow.h"
#include "btactioncollection.h"

//frontend includes
#include "frontend/keychooser/ckeychooser.h"
#include "frontend/profile/cprofilewindow.h"
#include "frontend/display/cwritedisplay.h"
#include "frontend/display/chtmlwritedisplay.h"

#include "backend/keys/cswordkey.h"

#include "util/cresmgr.h"
#include "util/directoryutil.h"

//Qt includes
#include <QToolBar>
#include <QMessageBox>
#include <QAction>

using namespace Profile;

CHTMLWriteWindow::CHTMLWriteWindow(QList<CSwordModuleInfo*> modules, CMDIArea* parent)
        : CPlainWriteWindow(modules, parent) {}

CHTMLWriteWindow::~CHTMLWriteWindow() {}

void CHTMLWriteWindow::initView() {
    CWriteDisplay* writeDisplay = CDisplay::createWriteInstance(this, CDisplay::HTMLDisplay);
    Q_ASSERT(writeDisplay);
    setDisplayWidget( writeDisplay );
    setCentralWidget( displayWidget()->view() );

    setMainToolBar( new QToolBar(this) );
    mainToolBar()->setAllowedAreas(Qt::TopToolBarArea);
    mainToolBar()->setFloatable(false);
    addToolBar(mainToolBar());

    setKeyChooser( CKeyChooser::createInstance(modules(), key(), mainToolBar()) );
    mainToolBar()->addWidget(keyChooser());
}

void CHTMLWriteWindow::initConnections() {
    CWriteWindow::initConnections();

    connect(keyChooser(), SIGNAL(keyChanged(CSwordKey*)), this, SLOT(lookupSwordKey(CSwordKey*)));
    connect(displayWidget()->connectionsProxy(), SIGNAL(textChanged()), this, SLOT(textChanged()) );
}

void CHTMLWriteWindow::initToolbars() {
    //setup the main toolbar
    m_actions.syncWindow = new QAction(
        util::filesystem::DirectoryUtil::getIcon(CResMgr::displaywindows::commentaryWindow::syncWindow::icon),
        tr("Sync with active Bible"),
        actionCollection()
    );
    m_actions.syncWindow->setCheckable(true);
    m_actions.syncWindow->setShortcut(CResMgr::displaywindows::commentaryWindow::syncWindow::accel);
    m_actions.syncWindow->setToolTip(tr("Synchronize (show the same verse) with the active Bible window"));
    actionCollection()->addAction(CResMgr::displaywindows::commentaryWindow::syncWindow::actionName, m_actions.syncWindow);
    mainToolBar()->addAction(m_actions.syncWindow);

    m_actions.saveText = new QAction(
        util::filesystem::DirectoryUtil::getIcon(CResMgr::displaywindows::writeWindow::saveText::icon),
        tr("Save text"),
        actionCollection()
    );
    m_actions.saveText->setShortcut(CResMgr::displaywindows::writeWindow::saveText::accel);
    m_actions.saveText->setToolTip( tr("Save text") );
    QObject::connect(m_actions.saveText, SIGNAL(triggered()), this, SLOT( saveCurrentText() ) );
    actionCollection()->addAction(CResMgr::displaywindows::writeWindow::saveText::actionName, m_actions.saveText);
    mainToolBar()->addAction(m_actions.saveText);


    m_actions.deleteEntry = new QAction(
        util::filesystem::DirectoryUtil::getIcon(CResMgr::displaywindows::writeWindow::deleteEntry::icon),
        tr("Delete current entry"),
        actionCollection()
    );
    m_actions.deleteEntry->setShortcut(CResMgr::displaywindows::writeWindow::deleteEntry::accel);
    m_actions.deleteEntry->setToolTip( tr("Delete current entry (no undo)") );
    QObject::connect(m_actions.deleteEntry, SIGNAL(triggered()), this, SLOT( deleteEntry() ) );
    actionCollection()->addAction(CResMgr::displaywindows::writeWindow::deleteEntry::actionName, m_actions.deleteEntry);
    mainToolBar()->addAction(m_actions.deleteEntry);

    m_actions.restoreText = new QAction(
        util::filesystem::DirectoryUtil::getIcon(CResMgr::displaywindows::writeWindow::restoreText::icon),
        tr("Restore original text"),
        actionCollection()
    );
    m_actions.restoreText->setShortcut(CResMgr::displaywindows::writeWindow::restoreText::accel);
    m_actions.restoreText->setToolTip( tr("Restore original text, new text will be lost") );
    QObject::connect(m_actions.restoreText, SIGNAL(triggered()), this, SLOT( restoreText() ) );
    actionCollection()->addAction(CResMgr::displaywindows::writeWindow::restoreText::actionName, m_actions.restoreText);
    mainToolBar()->addAction(m_actions.restoreText);

    //html formatting toolbar
    QToolBar* bar = new QToolBar(this);
    bar->setAllowedAreas(Qt::TopToolBarArea);
    bar->setFloatable(false);
    ((CWriteDisplay*)displayWidget())->setupToolbar( bar, actionCollection() );
    addToolBar(bar);
}

void CHTMLWriteWindow::storeProfileSettings( CProfileWindow* profileWindow ) {
    CWriteWindow::storeProfileSettings(profileWindow);
    profileWindow->setWindowSettings( m_actions.syncWindow->isChecked() );
}

void CHTMLWriteWindow::applyProfileSettings( CProfileWindow* profileWindow ) {
    CWriteWindow::applyProfileSettings(profileWindow);
    if (profileWindow->windowSettings()) {
        m_actions.syncWindow->setChecked(true);
    }
}

/** Is called when the current text was changed. */
void CHTMLWriteWindow::textChanged() {
    m_actions.saveText->setEnabled( ((CWriteDisplay*)displayWidget())->isModified() );
    m_actions.restoreText->setEnabled( ((CWriteDisplay*)displayWidget())->isModified() );
}

/** Loads the original text from the module. */
void CHTMLWriteWindow::restoreText() {
    lookupSwordKey(key());
    ((CWriteDisplay*)displayWidget())->setModified(false);
    textChanged();
}

bool CHTMLWriteWindow::syncAllowed() const {
    return m_actions.syncWindow->isChecked();
}

/** Saves the text for the current key. Directly writes the changed text into the module. */
void CHTMLWriteWindow::saveCurrentText( const QString& /*key*/ ) {
    QString t = ((CHTMLWriteDisplay*)displayWidget())->toHtml();
    //since t is a complete HTML page at the moment, strip away headers and footers of a HTML page
    QRegExp re("(?:<html.*>.+<body.*>)", Qt::CaseInsensitive); //remove headers, case insensitive
    re.setMinimal(true);
    t.replace(re, "");
    t.replace(QRegExp("</body></html>", Qt::CaseInsensitive), "");//remove footer

    const QString& oldKey = this->key()->key();
    if ( modules().first()->isWritable() ) {
        modules().first()->write(this->key(), t );
        this->key()->key( oldKey );

        ((CWriteDisplay*)displayWidget())->setModified(false);
        textChanged();
    }
    else {
        QMessageBox::critical( this, tr("Module not writable"),
                               QString::fromLatin1("<qt><b>%1</b><br />%2</qt>")
                               .arg( tr("Module is not writable.") )
                               .arg( tr("Either the module may not be edited, or you do not have write permission.") ) );
    }
}
