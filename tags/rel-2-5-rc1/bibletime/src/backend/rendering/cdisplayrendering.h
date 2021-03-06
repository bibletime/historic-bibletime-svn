/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef RENDERINGCDISPLAYRENDERING_H
#define RENDERINGCDISPLAYRENDERING_H

#include "backend/rendering/chtmlexportrendering.h"


namespace Rendering {

/** HTML rendering for the text display widgets.
 * @short Rendering for the html display widget.
 * @author The BibleTime team
 */

class CDisplayRendering : public CHTMLExportRendering {
    public:
        static const QString keyToHTMLAnchor(const QString& key);

        CDisplayRendering(
            CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(),
            CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults()
        );

    protected:
        virtual const QString entryLink( const KeyTreeItem& item, CSwordModuleInfo* const module );
        virtual const QString finishText( const QString&, KeyTree& tree );
};

}

#endif
