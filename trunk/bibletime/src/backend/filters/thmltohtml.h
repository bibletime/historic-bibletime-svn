/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef FILTERS_THMLTOHTML_H
#define FILTERS_THMLTOHTML_H

// Sword includes:
#include <swbuf.h>
#include <thmlhtml.h>


namespace Filters {

/**
  \brief ThML to HTML conversion filter.
*/
class ThmlToHtml: public sword::ThMLHTML {
    protected: /* Types: */
        class UserData: public sword::ThMLHTML::MyUserData {
            public:
                inline UserData(const sword::SWModule *module,
                                const sword::SWKey *key)
                    : sword::ThMLHTML::MyUserData(module, key),
                      inscriptRef(false), inFootnoteTag(false),
                      swordFootnote(1) {}

                bool inscriptRef;
                bool inFootnoteTag;
                unsigned short int swordFootnote;
        };

    public: /* Methods: */
        ThmlToHtml();

        /** Reimplemented from sword::OSISHTMLHREF. */
        virtual bool handleToken(sword::SWBuf &buf,
                                 const char *token,
                                 sword::BasicFilterUserData *userData);

        /** Reimplemented from sword::SWFilter. */
        virtual char processText(sword::SWBuf &buf,
                                 const sword::SWKey *key,
                                 const sword::SWModule *module = 0);

    protected: /* Methods: */
        /** Reimplemented from sword::OSISHTMLHREF. */
        virtual inline sword::BasicFilterUserData *createUserData(
                const sword::SWModule *module, const sword::SWKey *key)
        {
            return new UserData(module, key);
        }
};

} // namespace Filters

#endif
