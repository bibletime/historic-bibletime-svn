/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "backend/drivers/cswordbookmoduleinfo.h"

#include "backend/keys/cswordtreekey.h"

// Sword includes:
#include <treekey.h>
#include <treekeyidx.h>


CSwordBookModuleInfo::CSwordBookModuleInfo( sword::SWModule* module, CSwordBackend* const usedBackend )
        : CSwordModuleInfo(module, usedBackend),
        m_depth(-1) {}

CSwordBookModuleInfo::CSwordBookModuleInfo( const CSwordBookModuleInfo& module )
        : CSwordModuleInfo(module) {
    m_depth = module.m_depth;
}

CSwordBookModuleInfo::~CSwordBookModuleInfo() {}

int CSwordBookModuleInfo::depth() {
    if (m_depth == -1) {
        sword::TreeKeyIdx* key = tree();

        if (key) {
            key->root();
            computeDepth(key, 0);
        }
    }

    return m_depth;
}

void CSwordBookModuleInfo::computeDepth(sword::TreeKeyIdx* key, int level ) {
    std::string savedKey;
    //	savedKey = key->getFullName(); //sword 1.5.8
    savedKey = key->getText();

    if (level > m_depth) {
        m_depth = level;
    }

    if (key->hasChildren()) {
        key->firstChild();
        computeDepth(key, level + 1);

        key->setText( savedKey.c_str() );//return to the initial value
    }

    if (key->nextSibling()) {
        computeDepth(key, level);
    }
}

/** Returns a treekey filled with the structure of this module */
sword::TreeKeyIdx* CSwordBookModuleInfo::tree() const {
    sword::TreeKeyIdx* treeKey = dynamic_cast<sword::TreeKeyIdx*>((sword::SWKey*) * (module()));
    Q_ASSERT(treeKey);
    return treeKey;
}
