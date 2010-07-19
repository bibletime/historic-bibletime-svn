/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef FILTERS_TEITOHTML_H
#define FILTERS_TEITOHTML_H

// Sword includes:
#include <teihtmlhref.h>
#include <swbuf.h>

namespace Filters {

/**
  \brief TEI to HTML conversion filter.
*/
class TeiToHtml: public sword::TEIHTMLHREF {
    public: /* Methods: */
        TeiToHtml();

        /** Reimplemented from sword::OSISHTMLHREF. */
        virtual bool handleToken(sword::SWBuf &buf, const char *token,
                                 sword::BasicFilterUserData *userData);

    private: /* Methods: */
        void renderReference(const char *osisRef, sword::SWBuf &buf,
                             sword::BasicFilterUserData *myUserData);
};

} // namespace Filters

#endif
