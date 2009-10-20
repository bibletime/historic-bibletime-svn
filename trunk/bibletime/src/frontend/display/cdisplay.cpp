/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "frontend/display/cdisplay.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QTimer>
#include "backend/managers/creferencemanager.h"
#include "frontend/display/bthtmlreaddisplay.h"
#include "frontend/display/chtmlwritedisplay.h"
#include "frontend/display/cplainwritedisplay.h"
#include "util/tool.h"


typedef BtHtmlReadDisplay HTMLREADDISPLAY;

CDisplayConnections::CDisplayConnections( CDisplay* display ) : m_display(display) {}

void CDisplayConnections::selectAll() {
    m_display->selectAll();
}

void CDisplayConnections::saveAsHTML() {
    m_display->save(CDisplay::HTMLText, CDisplay::Document);
}

void CDisplayConnections::saveAsPlain() {
    m_display->save(CDisplay::PlainText, CDisplay::Document);
}

/** Emits the signal. */
void CDisplayConnections::emitReferenceClicked( const QString& module, const QString& key) {
    qDebug() << "CDisplayConnections::emitReferenceClicked";
    qDebug() << "Module: " << module << " key: " << key;
    emit referenceClicked( module, key );
}

/** Emits the signal. */
void CDisplayConnections::emitReferenceDropped( const QString& key) {
    emit referenceDropped(key);
}

/** Emits the signal. */
void CDisplayConnections::emitTextChanged() {
    emit textChanged();
}

void CDisplayConnections::copyAll() {
    m_display->copy(CDisplay::PlainText, CDisplay::Document);
}

/** No descriptions */
void CDisplayConnections::copySelection() {
    qWarning("copyign the selected text");
    m_display->copy(CDisplay::PlainText, CDisplay::SelectedText);
}

void CDisplayConnections::printAll(CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions) {
    m_display->print(CDisplay::Document, displayOptions, filterOptions);
}

void CDisplayConnections::printAnchorWithText(CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions) {
    m_display->print(CDisplay::AnchorWithText, displayOptions, filterOptions);
}

void CDisplayConnections::copyAnchorOnly() {
    m_display->copy(CDisplay::PlainText, CDisplay::AnchorOnly);
}

void CDisplayConnections::copyAnchorTextOnly() {
    m_display->copy(CDisplay::PlainText, CDisplay::AnchorTextOnly);
}

void CDisplayConnections::copyAnchorWithText() {
    m_display->copy(CDisplay::PlainText, CDisplay::AnchorWithText);
}

void CDisplayConnections::saveAnchorWithText() {
    m_display->save(CDisplay::PlainText, CDisplay::AnchorWithText);
}

void CDisplayConnections::clear() {
    m_display->setText(QString::null);
}

void CDisplayConnections::zoomIn() {
    m_display->zoomIn();
}

void CDisplayConnections::zoomOut() {
    m_display->zoomOut();
}

void CDisplayConnections::openFindTextDialog() {
    m_display->openFindTextDialog();
}


/*----------------------*/

CReadDisplay* CDisplay::createReadInstance( CReadWindow* readWindow, QWidget* parent ) {
    return new HTMLREADDISPLAY(readWindow, parent);
}

CWriteDisplay* CDisplay::createWriteInstance( CWriteWindow* writeWindow, const CWriteDisplay::WriteDisplayType& type, QWidget* parent ) {
    //  qWarning("CDisplay::createWriteInstance");
    if (type == PlainTextDisplay) {
        return new CPlainWriteDisplay(writeWindow, parent);
    }
    else {
        return new CHTMLWriteDisplay(writeWindow, parent);
    };
}


CDisplay::CDisplay(CDisplayWindow* parent) :
        m_parentWindow(parent),
        m_connections( new CDisplayConnections( this ) ),
        m_popup(0) {}

CDisplay::~CDisplay() {
    delete m_connections;
}

bool CDisplay::copy( const CDisplay::TextType format, const CDisplay::TextPart part  ) {
    QApplication::clipboard()->setText( this->text(format, part) );
    return true;
}

bool CDisplay::save( const CDisplay::TextType format, const CDisplay::TextPart part ) {
    //  qWarning("CDisplay::save( const CDisplay::TextType format, const CDisplay::TextPart part  )");
    const QString content = text(format, part);
    QString filter = QString::null;

    switch (format) {
        case HTMLText:
            filter = QObject::tr("HTML files") + QString(" (*.html *.htm);;") + QObject::tr("All files") + QString(" (*.*)");
            break;
        case PlainText:
            filter = QObject::tr("Text files") + QString(" (*.txt);;") + QObject::tr("All files") + QString(" (*.*)");
            break;
    }

    const QString filename = QFileDialog::getSaveFileName(0, QObject::tr("Save document ..."), "", filter);

    if (!filename.isEmpty()) {
        util::tool::savePlainFile(filename, content);
    }
    return true;
}

/** Emits the signal which used when a reference was clicked. */
void CDisplay::emitReferenceClicked( const QString& reference ) {
    QString module, key;
    CReferenceManager::Type type;
    CReferenceManager::decodeHyperlink(reference, module, key, type);
    if (module.isEmpty()) {
        module = CReferenceManager::preferredModule( type );
    }
    m_connections->emitReferenceClicked(module, key);
}

/** Used when a reference was dropped onto the widget. */
void CDisplay::emitReferenceDropped( const QString& reference ) {
    QString module;
    QString key;
    CReferenceManager::Type type;
    CReferenceManager::decodeHyperlink(reference, module, key, type);
    m_connections->emitReferenceDropped(key);
}

/** Returns the connections obect used for signas and slots. */
CDisplayConnections* CDisplay::connectionsProxy() const {
    return m_connections;
}

CDisplayWindow* CDisplay::parentWindow() const {
    return m_parentWindow;
}

/** Installs the popup which should be opened when the right mouse button was pressed. */
void CDisplay::installPopup( QMenu* popup ) {
    m_popup = popup;
}

/** Returns the popup menu which was set by installPopupMenu() */
QMenu* CDisplay::installedPopup() {
    return m_popup;
}

