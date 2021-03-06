/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "chtmlreaddisplay.h"
#include "chtmlreaddisplay.moc"

#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/creadwindow.h"
#include "backend/managers/creferencemanager.h"
#include "backend/keys/cswordkey.h"

#include "backend/config/cbtconfig.h"
#include "frontend/cdragdrop.h"
#include "frontend/cinfodisplay.h"

#include "util/ctoolclass.h"
#include "util/cpointers.h"
#include <boost/scoped_ptr.hpp>

#include <QCursor>
#include <QScrollArea>
#include <QDrag>
#include <QMenu>
#include <QLayout>
#include <QTimer>
#include <QDebug>
#include <QApplication>

#include <ktoolinvocation.h>
#include <khtmlview.h>
#include <khtml_events.h>
#include <dom/dom2_range.h>
#include <dom/html_element.h>
#include <dom/dom2_traversal.h>
#include <dom/html_document.h>

using namespace InfoDisplay;

CHTMLReadDisplay::CHTMLReadDisplay(CReadWindow* readWindow, QWidget* parentWidget)
	: KHTMLPart(m_view = new CHTMLReadDisplayView(
			this, parentWidget ? parentWidget : readWindow), readWindow ? readWindow : parentWidget),
	CReadDisplay(readWindow),
	m_magTimerId(0),
	m_currentAnchorCache(QString::null)
{
	view()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	KHTMLPart::view()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	setDNDEnabled(false);
	setJavaEnabled(false);
	setJScriptEnabled(false);
	setPluginsEnabled(false);

	//m_view->setDragAutoScroll(false); // TODO: doesn't exist in qt4; is needed?
	m_urlWorkaroundData.doWorkaround = false;
}

CHTMLReadDisplay::~CHTMLReadDisplay() {}

const QString CHTMLReadDisplay::text( const CDisplay::TextType format, const CDisplay::TextPart part) {
	switch (part) {
		case Document: {
			if (format == HTMLText) {
				return document().toHTML();
			}
			else {
				//return htmlDocument().body().innerText().string().toLatin1();
				CDisplayWindow* window = parentWindow();
				CSwordKey* const key = window->key();
				CSwordModuleInfo* module = key->module();
				//This is never used for Bibles, so it is not implemented for
				//them.  If it should be, see CReadDisplay::print() for example
				//code.
				Q_ASSERT(module->type() == CSwordModuleInfo::Lexicon ||
						module->type() == CSwordModuleInfo::Commentary ||
						module->type() == CSwordModuleInfo::GenericBook);
				if (module->type() == CSwordModuleInfo::Lexicon ||
						module->type() == CSwordModuleInfo::Commentary ||
						module->type() == CSwordModuleInfo::GenericBook){
					//TODO: This is a BAD HACK, we have to fnd a better solution to manage the settings now
					CSwordBackend::FilterOptions filterOptions;
					filterOptions.footnotes = false;
					filterOptions.strongNumbers = false;
					filterOptions.morphTags = false;
					filterOptions.lemmas = false;
					filterOptions.scriptureReferences = false;
					filterOptions.textualVariants = false;

					CPointers::backend()->setFilterOptions(filterOptions);

					return QString(key->strippedText()).append("\n(")
						.append(key->key())
						.append(", ")
						.append(key->module()->name())
						.append(")");
				}
			}
		}

		case SelectedText: {
			if (!hasSelection()) {
				return QString::null;
			}
			else if (format == HTMLText) {
				DOM::Range range = selection();
				return range.toHTML().string();
			}
			else { //plain text requested
				return selectedText();
			}
		}

		case AnchorOnly: {
			QString moduleName;
			QString keyName;
			CReferenceManager::Type type;
			CReferenceManager::decodeHyperlink(activeAnchor(), moduleName, keyName, type);

			return keyName;
		}

		case AnchorTextOnly: {
			QString moduleName;
			QString keyName;
			CReferenceManager::Type type;
			CReferenceManager::decodeHyperlink(activeAnchor(), moduleName, keyName, type);

			if (CSwordModuleInfo* module = backend()->findModuleByName(moduleName)) {
				boost::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
				key->key( keyName );

				return key->strippedText();
			}
			return QString::null;
		}

		case AnchorWithText: {
			QString moduleName;
			QString keyName;
			CReferenceManager::Type type;
			CReferenceManager::decodeHyperlink(activeAnchor(), moduleName, keyName, type);

			if (CSwordModuleInfo* module = backend()->findModuleByName(moduleName)) {
				boost::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
				key->key( keyName );

				//TODO: This is a BAD HACK, we have to fnd a better solution to manage the settings now
				CSwordBackend::FilterOptions filterOptions;
				filterOptions.footnotes = false;
				filterOptions.strongNumbers = false;
				filterOptions.morphTags = false;
				filterOptions.lemmas = false;
				filterOptions.scriptureReferences = false;
				filterOptions.textualVariants = false;

				CPointers::backend()->setFilterOptions(filterOptions);

				return QString(key->strippedText()).append("\n(")
					.append(key->key())
					.append(", ")
					.append(key->module()->name())
					.append(")");
				/*    ("%1\n(%2, %3)")
					.arg()
					.arg(key->key())
					.arg(key->module()->name());*/
			}
			return QString::null;
		}
		default:
		return QString::null;
	}
}

void CHTMLReadDisplay::setText( const QString& newText ) {
	begin();
	write(newText);
	end();
}

/** No descriptions */
const bool CHTMLReadDisplay::hasSelection() {
	return KHTMLPart::hasSelection();
}


/** Reimplementation. */
QScrollArea* CHTMLReadDisplay::view() {
	//return KHTMLPart::view();
	return m_view;
}

void CHTMLReadDisplay::selectAll() {
	KHTMLPart::selectAll();
}

/** No descriptions */
void CHTMLReadDisplay::moveToAnchor( const QString& anchor ) {
	//m_currentAnchorCache = anchor;

	//This is an ugly hack to work around a KDE problem in KDE including 3.3.1 (no later versions tested so far)
	//TODO for kde4 port: maybe we can remove this?
	//QTimer::singleShot(0, this, SLOT(slotGoToAnchor()));
	slotGoToAnchor(anchor);
	// instead of:
	//  slotGoToAnchor();
}

/** Reimplementation from KHTMLPart. Called when user selects (clicks) an URL. Returns whether a new
* URL was opened or not.
*/
bool CHTMLReadDisplay::urlSelected( const QString& url, int button, int state,
									const QString& _target,
									const KParts::OpenUrlArguments& args,
									const KParts::BrowserArguments& b_args
								)
{
	qDebug("CHTMLReadDisplay::urlSelected");
	m_urlWorkaroundData.doWorkaround = false;
	qDebug() << "clicked: " << url;
	if (!url.isEmpty() && CReferenceManager::isHyperlink(url)) {
		QString module;
		QString key;
		CReferenceManager::Type type;

		CReferenceManager::decodeHyperlink(url, module, key, type);
		if (module.isEmpty()) {
			module = CReferenceManager::preferredModule( type );
		}

		// we have to use this workaround, otherwise the widget would scroll because it was interrupted
		// between mouseClick and mouseRelease (I guess)
		// TODO: is this still true with KDE4?
		m_urlWorkaroundData.doWorkaround = true;
		m_urlWorkaroundData.url = url;
		m_urlWorkaroundData.state = state;
		m_urlWorkaroundData.button = button;
		m_urlWorkaroundData.target = _target;
		m_urlWorkaroundData.args = args;
		m_urlWorkaroundData.module = module;
		m_urlWorkaroundData.key = key;
	}
	else if (!url.isEmpty() && (url.left(1) == "#")) { //anchor
		moveToAnchor(url.mid(1));
	}
	else if (url.left(7) == "http://") { //open the bowser configured by kdeb
		KToolInvocation::invokeBrowser( url ); //TODO: Not yet tested
	}

	return KHTMLPart::urlSelected(url, button, state, _target, args, b_args);
}

/** Reimplementation. */
void CHTMLReadDisplay::khtmlMouseReleaseEvent( khtml::MouseReleaseEvent* event ) {
	qDebug("CHTMLReadDisplay::khtmlMouseReleaseEvent");
	KHTMLPart::khtmlMouseReleaseEvent(event);

	m_dndData.mousePressed = false;
	m_dndData.isDragging = false;
	m_dndData.node = DOM::Node();
	m_dndData.anchor = DOM::DOMString();

	if (m_urlWorkaroundData.doWorkaround) {
		qDebug("do workaround");
		m_urlWorkaroundData.doWorkaround = false;
		qDebug("will emit connectionsProxy()->emitReferenceClicked");
		connectionsProxy()->emitReferenceClicked(
			m_urlWorkaroundData.module,
			m_urlWorkaroundData.key
		);
	}
}

void CHTMLReadDisplay::khtmlMousePressEvent( khtml::MousePressEvent* event ) {
	qDebug("CHTMLReadDisplay::khtmlMousePressEvent");
	m_dndData.node = DOM::Node();
	m_dndData.anchor = DOM::DOMString();
	m_dndData.mousePressed = false;
	m_dndData.isDragging = false;

	if (event->qmouseEvent()->button() == Qt::RightButton) {
		qDebug("Right button");
		DOM::Node tmpNode = event->innerNode();
		DOM::Node attr;
		m_nodeInfo[CDisplay::Lemma] = QString::null;

		do {
			if (!tmpNode.isNull() && (tmpNode.nodeType() == 
						DOM::Node::ELEMENT_NODE) && tmpNode.hasAttributes()) {
				attr = tmpNode.attributes().getNamedItem("lemma");
				if (!attr.isNull()) {
					m_nodeInfo[ CDisplay::Lemma ] = attr.nodeValue().string();
					break;
				}
			}
			tmpNode = tmpNode.parentNode();
		} while ( !tmpNode.isNull() );
		
		setActiveAnchor( event->url().string() );
	}
	else if (event->qmouseEvent()->button() == Qt::LeftButton) {
		qDebug("Left button");
		m_dndData.node = event->innerNode();
		m_dndData.anchor = event->url();
		m_dndData.mousePressed = true;
		m_dndData.isDragging = false;
		m_dndData.startPos = QPoint(event->x(), event->y());
		m_dndData.selection = selectedText();

		if (!m_dndData.node.isNull()) { //we drag a valid link
			m_dndData.dragType = DNDData::Link;
		}
	}

	KHTMLPart::khtmlMousePressEvent(event);
}

/** Reimplementation for our drag&drop system. Also needed for the mouse tracking */
void CHTMLReadDisplay::khtmlMouseMoveEvent( khtml::MouseMoveEvent* e ) {
	if( e->qmouseEvent()->buttons() & Qt::LeftButton == Qt::LeftButton) { //left mouse button pressed
		const int delay = qApp->startDragDistance();
		QPoint newPos = QPoint(e->x(), e->y());
	

		//TODO: this should be inspected
		if ( (newPos.x() > m_dndData.startPos.x()+delay || newPos.x() < (m_dndData.startPos.x()-delay) ||
				newPos.y() > m_dndData.startPos.y()+delay || newPos.y() < (m_dndData.startPos.y()-delay)) &&
				!m_dndData.isDragging && m_dndData.mousePressed  ) {
			//Q3DragObject* d = 0;
			QDrag* d = 0;
			if (!m_dndData.anchor.isEmpty() && (m_dndData.dragType == DNDData::Link) && !m_dndData.node.isNull() ) {
				// create a new bookmark drag!
				QString moduleName = QString::null;
				QString keyName = QString::null;
				CReferenceManager::Type type;
				if ( !CReferenceManager::decodeHyperlink(m_dndData.anchor.string(), moduleName, keyName, type) )
					return;

				d = new QDrag(KHTMLPart::view()->viewport());
				BTMimeData* mimedata = new BTMimeData(moduleName, keyName, QString::null);
				d->setMimeData(mimedata);
				//add real Bible text from module/key
				if (CSwordModuleInfo* module = backend()->findModuleByName(moduleName)) {
					boost::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
					key->key( keyName );
					mimedata->setText(key->strippedText()); // This works across applications!
				}
			}
			else if ((m_dndData.dragType == DNDData::Text) && !m_dndData.selection.isEmpty()) {
				d = new QDrag(KHTMLPart::view()->viewport());
				BTMimeData* mimedata = new BTMimeData(m_dndData.selection);
				d->setMimeData(mimedata);
			}
	
			if (d) {
				m_dndData.isDragging = true;
				m_dndData.mousePressed = false;
	
				//first make a virtual mouse click to end the selection, if it's in progress
				QMouseEvent e(QEvent::MouseButtonRelease, QPoint(0,0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
				QApplication::sendEvent(view()->viewport(), &e);
				d->exec(Qt::CopyAction, Qt::CopyAction);
			}
		}
	}
	else if (getMouseTracking() && !(e->qmouseEvent()->modifiers() & Qt::ShiftModifier == Qt::ShiftModifier)) { 
		//no mouse button pressed and tracking enabled
		DOM::Node node = e->innerNode();
		//if no link was under the mouse try to find a title attribute
		if (!node.isNull() && (m_previousEventNode != node)) {
			// we want to avoid processing the node again
			// After some millisecs the new timer activates the Mag window update, see timerEvent()
			// SHIFT key not pressed, so we start timer
			if ( !(e->qmouseEvent()->modifiers() & Qt::ShiftModifier)) { 
				// QObject has simple timer
				killTimer(m_magTimerId);
				m_magTimerId = startTimer( CBTConfig::get(CBTConfig::magDelay) );
			}
	
			m_previousEventNode = node;
		}
	}
		
	KHTMLPart::khtmlMouseMoveEvent(e);
}

/** The Mag window update happens here if the mouse has not moved to another node after starting the timer.*/
void CHTMLReadDisplay::timerEvent( QTimerEvent* /*e*/ ) {
	killTimer(m_magTimerId);
	DOM::Node currentNode = nodeUnderMouse();
	CInfoDisplay::ListInfoData infoList;
	
	// Process the node under cursor if it is the same as at the start of the timer
	if (!currentNode.isNull() && (currentNode != m_previousEventNode) && this->view()->underMouse()) {
		DOM::Node attr;
		do {
			if (!currentNode.isNull() && (currentNode.nodeType() == DOM::Node::ELEMENT_NODE) && currentNode.hasAttributes()) { //found right node
				attr = currentNode.attributes().getNamedItem("note");
				if (!attr.isNull()) {
					infoList.append( qMakePair(CInfoDisplay::Footnote, attr.nodeValue().string()) );
				}
	
				attr = currentNode.attributes().getNamedItem("lemma");
				if (!attr.isNull()) {
					infoList.append( qMakePair(CInfoDisplay::Lemma, attr.nodeValue().string()) );
				}
	
				attr = currentNode.attributes().getNamedItem("morph");
				if (!attr.isNull()) {
					infoList.append( qMakePair(CInfoDisplay::Morph, attr.nodeValue().string()) );
				}
	
				attr = currentNode.attributes().getNamedItem("expansion");
				if (!attr.isNull()) {
					infoList.append( qMakePair(CInfoDisplay::Abbreviation, attr.nodeValue().string()) );
				}
				
				attr = currentNode.attributes().getNamedItem("crossrefs");
				if (!attr.isNull()) {
					infoList.append( qMakePair(CInfoDisplay::CrossReference, attr.nodeValue().string()) );
				}
			}
	
			currentNode = currentNode.parentNode();
			if (!currentNode.isNull() && currentNode.hasAttributes()) {
				attr = currentNode.attributes().getNamedItem("class");
				if (!attr.isNull() && (attr.nodeValue().string() == "entry") || (attr.nodeValue().string() == "currententry") ) {
					break;
				}
			}
		}
		while ( !currentNode.isNull() );
	}
	
	// Update the mag if there is new content
	if (!(infoList.isEmpty())) {
		CPointers::infoDisplay()->setInfo(infoList);
	}

}

// ---------------------

CHTMLReadDisplayView::CHTMLReadDisplayView(CHTMLReadDisplay* displayWidget, QWidget* parent) : KHTMLView(displayWidget, parent), m_display(displayWidget) {
	viewport()->setAcceptDrops(true);
	setMarginWidth(4);
	setMarginHeight(4);
}


/** Opens the popupmenu at the given position. */
void CHTMLReadDisplayView::popupMenu( const QString& url, const QPoint& pos) {
	if (!url.isEmpty()) {
		m_display->setActiveAnchor(url);
	}
	if (QMenu* popup = m_display->installedPopup()) {
		popup->exec(pos);
	}
}

/** Reimplementation from QScrollArea. Sets the right slots */
bool CHTMLReadDisplayView::event(QEvent* e) {
	//TODO: this MUST be tested (as other context menus and DnD system) when the app runs)
	if (e->type() == QEvent::Polish) {
		connect( part(), SIGNAL(popupMenu(const QString&, const QPoint&)),
			this, SLOT(popupMenu(const QString&, const QPoint&)));
	}
	return KHTMLView::event(e);
}

/** Reimplementation from QScrollArea. */
void CHTMLReadDisplayView::dropEvent( QDropEvent* e ) {
	//if (CDragDropMgr::canDecode(e) && CDragDropMgr::dndType(e) == CDragDropMgr::Item::Bookmark) {
	if (e->mimeData()->hasFormat("BibleTime/Bookmark")) {
		//CDragDropMgr::ItemList dndItems = CDragDropMgr::decode(e);
		//see docs for BTMimeData and QMimeData
		BookmarkItem item = (qobject_cast<const BTMimeData*>(e->mimeData()))->bookmark();
		
		//TODO: acceptAction() is no more in qt4. This should be tested. See also dragEnterEvent.
		e->acceptProposedAction();
		//bookmarkkey: QString
		m_display->connectionsProxy()->emitReferenceDropped(item.key());
		return;
	};

	//don't accept the action!
	//e->acceptAction(false);
	e->ignore();
}

/** Reimplementation from QScrollArea. */
void CHTMLReadDisplayView::dragEnterEvent( QDragEnterEvent* e ) {
	//if (CDragDropMgr::canDecode(e) && CDragDropMgr::dndType(e) == CDragDropMgr::Item::Bookmark) {
	if (e->mimeData()->hasFormat("BibleTime/Bookmark")) {
		e->acceptProposedAction();
		return;
	}
	
	//e->acceptAction(false);
	e->ignore();
}

/*!
\fn CHTMLReadDisplay::slotGoToAnchor()
*/
void CHTMLReadDisplay::slotGoToAnchor(const QString& anchor) {
	qDebug("CHTMLReadDisplay::slotGoToAnchor");
	qDebug() << "anchor:" << anchor;
	if (!gotoAnchor(anchor) ) {
		qWarning() << "No such anchor:" << anchor;
	}
	qDebug("CHTMLReadDisplay::slotGoToAnchor end");
}

void CHTMLReadDisplay::zoomIn() {
	setFontScaleFactor( (int)((float)fontScaleFactor()*1.1) );
}

void CHTMLReadDisplay::zoomOut() {
	setFontScaleFactor( (int)((float)fontScaleFactor()*(1.0/1.1)) );
}

void CHTMLReadDisplay::openFindTextDialog() {
	findText();
}
