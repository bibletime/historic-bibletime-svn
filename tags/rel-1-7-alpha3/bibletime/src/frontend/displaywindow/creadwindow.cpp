/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



//BibleTime includes
#include "creadwindow.h"

#include "frontend/display/chtmlreaddisplay.h"

#include "backend/rendering/centrydisplay.h"
#include "backend/rendering/cdisplayrendering.h"
#include "backend/keys/cswordkey.h"
#include "backend/keys/cswordversekey.h"

#include "frontend/cexportmanager.h"
#include "frontend/cmdiarea.h"
#include "frontend/profile/cprofilewindow.h"
#include "frontend/searchdialog/csearchdialog.h"


#include <kactioncollection.h>


#include <QResizeEvent>
#include <QMdiSubWindow>

using namespace Profile;

CReadWindow::CReadWindow(QList<CSwordModuleInfo*> modules, CMDIArea* parent)
	: CDisplayWindow(modules,parent),
	m_displayWidget(0)
{
	qDebug("CReadWindow::CReadWindow");
	//   installEventFilter(this);
}

CReadWindow::~CReadWindow() {
	//  qWarning("destructor of CReadWindow");
}

/** Sets the display widget of this display window. */
void CReadWindow::setDisplayWidget( CDisplay* newDisplay ) {
	Q_ASSERT(dynamic_cast<CReadDisplay*>(newDisplay));
	CDisplayWindow::setDisplayWidget(newDisplay);
	if (m_displayWidget) {
		disconnect(m_displayWidget->connectionsProxy(), SIGNAL(referenceClicked(const QString&, const QString&)), this, SLOT(lookupModKey(const QString&, const QString&)));
		disconnect(m_displayWidget->connectionsProxy(), SIGNAL(referenceDropped(const QString&)), this, SLOT(lookupKey(const QString&)));
		
		CHTMLReadDisplay* v = dynamic_cast<CHTMLReadDisplay*>(m_displayWidget);
		if (v) {
			QObject::disconnect(v, SIGNAL(completed()), this, SLOT(slotMoveToAnchor()) );
		}

	}

	m_displayWidget = (CReadDisplay*)newDisplay;
	connect(
		m_displayWidget->connectionsProxy(),
		SIGNAL(referenceClicked(const QString&, const QString&)),
		this,
		SLOT(lookupModKey(const QString&, const QString&))
	);

	connect(
		m_displayWidget->connectionsProxy(),
		SIGNAL(referenceDropped(const QString&)),
		this,
		SLOT(lookupKey(const QString&))
	);
	CHTMLReadDisplay* v = dynamic_cast<CHTMLReadDisplay*>(m_displayWidget);
	if (v) {
		QObject::connect(v, SIGNAL(completed()), this, SLOT(slotMoveToAnchor()) );
	}
}

/** Lookup the given entry. */
void CReadWindow::lookupSwordKey( CSwordKey* newKey ) {
	qDebug() << "CReadWindow::lookup newKey" << newKey->key();
	Q_ASSERT(newKey);

	using namespace Rendering;

	Q_ASSERT(isReady() && newKey && modules().first());
	if (!isReady() || !newKey || !modules().first()) {
		return;
	}

	//next-TODO: how about options?
	Q_ASSERT(modules().first()->getDisplay());
	CEntryDisplay* display = modules().first()->getDisplay();
	if (display) { //do we have a display object?
		displayWidget()->setText(
			display->text(
				modules(),
				newKey->key(),
				displayOptions(),
				filterOptions()
			)
		);
	}

	if (key() != newKey) {
		key()->key(newKey->key());
	}

	setCaption( windowCaption() );

	// moving to anchor happens in slotMoveToAnchor which catches the completed() signal from KHTMLPart	

	qDebug() << "CReadWindow::lookup end, key is :" << newKey->key();
}

void CReadWindow::slotMoveToAnchor()
{
	qDebug("CReadWindow::slotMoveToAnchor");

	//Ugly HACK to get scrollbar working when opening a display window;
	KHTMLPart* part = dynamic_cast<KHTMLPart*>(displayWidget());
	if (part) {
		KHTMLView* view = part->view();
		view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		view->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}

	((CReadDisplay*)displayWidget())->moveToAnchor( Rendering::CDisplayRendering::keyToHTMLAnchor(key()->key()) );
}

/** Store the settings of this window in the given CProfileWindow object. */
void CReadWindow::storeProfileSettings(CProfileWindow * const settings) {
	QRect rect;
	rect.setX(parentWidget()->x());
	rect.setY(parentWidget()->y());
	rect.setWidth(width());
	rect.setHeight(height());
	settings->setGeometry(rect);

	// settings->setScrollbarPositions( m_htmlWidget->view()->horizontalScrollBar()->value(), m_htmlWidget->view()->verticalScrollBar()->value() );
	settings->setType(modules().first()->type());
	settings->setMaximized(isMaximized() || parentWidget()->isMaximized());
	settings->setFocus( (this == dynamic_cast<CReadWindow*>(mdi()->activeSubWindow()) ) ); //set property to true if this window is the active one.

	if (key()) {
		sword::VerseKey* vk = dynamic_cast<sword::VerseKey*>(key());
		QString oldLang;
		if (vk) {
			oldLang = QString(vk->getLocale());
			vk->setLocale("en"); //save english locale names as default!
		}
		settings->setKey( key()->key() );
		if (vk) {
			vk->setLocale(oldLang.toLatin1());
		}
	}

	QStringList mods;

	QList<CSwordModuleInfo*> allMods = modules();
	QList<CSwordModuleInfo*>::iterator end_it = allMods.end();
	for (QList<CSwordModuleInfo*>::iterator it(allMods.begin()); it != end_it; ++it) {
		mods.append((*it)->name());
	}
	settings->setModules(mods);
}

void CReadWindow::applyProfileSettings(CProfileWindow * const settings) {
	//  parentWidget()->setUpdatesEnabled(false);
	setUpdatesEnabled(false);

	if (settings->maximized()) { //maximize this window
		// Use parentWidget() to call showMaximized. Otherwise we'd get lot's of X11 errors
		parentWidget()->showMaximized();
	}
	else {
		const QRect rect = settings->geometry();
		parentWidget()->resize(rect.width(), rect.height());
		parentWidget()->move(rect.x(), rect.y());
	}

	setUpdatesEnabled(true);
}

void CReadWindow::insertKeyboardActions( KActionCollection* const ) {}

/** No descriptions */
void CReadWindow::copyDisplayedText() {
	CExportManager mgr(QString::null);
	mgr.copyKey(key(), CExportManager::Text, true);
}


/*!
    \fn CReadWindow::resizeEvent(QResizeEvent* e)
 */
void CReadWindow::resizeEvent(QResizeEvent* /*e*/) {
	if (displayWidget()) {
		((CReadDisplay*)displayWidget())->moveToAnchor(Rendering::CDisplayRendering::keyToHTMLAnchor(key()->key()));
	}
}

void CReadWindow::openSearchStrongsDialog() {
//	qWarning("looking for lemma %s",  displayWidget()->getCurrentNodeInfo()[CDisplay::Lemma].latin1() );
	QString searchText = QString::null;
	
	if (displayWidget()->getCurrentNodeInfo()[CDisplay::Lemma] != QString::null) {
		searchText.append("strong:").append(displayWidget()->getCurrentNodeInfo() [CDisplay::Lemma]);
	}
	
	Search::CSearchDialog::openDialog( modules(), searchText, 0 );
}
