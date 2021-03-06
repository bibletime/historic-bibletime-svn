/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



//BibleTime includes
#include "creaddisplay.h"

#include "backend/drivers/cswordmoduleinfo.h"
#include "backend/drivers/cswordbiblemoduleinfo.h"
#include "backend/drivers/cswordlexiconmoduleinfo.h"
#include "backend/drivers/cswordbookmoduleinfo.h"
#include "backend/keys/cswordkey.h"
#include "backend/keys/cswordversekey.h"
#include "backend/keys/cswordtreekey.h"
#include "backend/keys/cswordldkey.h"

#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/creadwindow.h"

#include "frontend/cexportmanager.h"

#include <boost/scoped_ptr.hpp>

//KDE includes


CReadDisplay::CReadDisplay(CReadWindow* readWindow) :
        CDisplay(readWindow),
        m_activeAnchor(QString::null),
        m_useMouseTracking(true) {}

CReadDisplay::~CReadDisplay() {}

/** Returns the current active anchor. */
const QString& CReadDisplay::activeAnchor() {
    return m_activeAnchor;
}

/** Sets the current anchor to the parameter. */
void CReadDisplay::setActiveAnchor( const QString& anchor ) {
    m_activeAnchor = anchor;
}


/** Returns true if the display has an active anchor. */
bool CReadDisplay::hasActiveAnchor() {
    return !activeAnchor().isEmpty();
}


void CReadDisplay::print(const CDisplay::TextPart type, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions) {
    CDisplayWindow* window = parentWindow();
    CSwordKey* const key = window->key();
    CSwordModuleInfo* module = key->module();


    CExportManager mgr(QObject::tr("Print keys"), false, QString::null, parentWindow()->filterOptions(), parentWindow()->displayOptions());

    switch (type) {
        case Document: {
            if (module->type() == CSwordModuleInfo::Bible) {
                CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(key);

                CSwordVerseKey startKey(*vk);
                startKey.Verse(1);

                CSwordVerseKey stopKey(*vk);

                CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module);
                if (bible) {
                    stopKey.Verse( bible->verseCount( bible->bookNumber(startKey.book()), startKey.Chapter() ) );
                }

                mgr.printKey(module, startKey.key(), stopKey.key(), displayOptions, filterOptions);
            }
            else if (module->type() == CSwordModuleInfo::Lexicon || module->type() == CSwordModuleInfo::Commentary ) {
                mgr.printKey(module, key->key(), key->key(), displayOptions, filterOptions);
            }
            else if (module->type() == CSwordModuleInfo::GenericBook) {
                CSwordTreeKey* tree = dynamic_cast<CSwordTreeKey*>(key);

                CSwordTreeKey startKey(*tree);
                //        while (startKey.previousSibling()) { // go to first sibling on this level!
                //        }

                CSwordTreeKey stopKey(*tree);
                //    if (CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(module)) {
                //          while ( stopKey.nextSibling() ) { //go to last displayed sibling!
                //          }
                //        }
                mgr.printKey(module, startKey.key(), stopKey.key(), displayOptions, filterOptions);
            }
        }

        case AnchorWithText: {
            if (hasActiveAnchor()) {
                mgr.printByHyperlink( activeAnchor(), displayOptions, filterOptions );
            }
        }

        default:
            break;
    }
}

