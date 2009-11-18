/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef INFODISPLAYCROSSREFRENDERING_H
#define INFODISPLAYCROSSREFRENDERING_H

#include "backend/rendering/chtmlexportrendering.h"


namespace InfoDisplay {

class CrossRefRendering : public Rendering::CHTMLExportRendering {
    protected:
        friend class CInfoDisplay;

        CrossRefRendering(
            CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(),
            CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults()
        );

        virtual const QString entryLink( const KeyTreeItem& item, CSwordModuleInfo* module );
        virtual const QString finishText( const QString&, KeyTree& tree );
};


}

#endif
