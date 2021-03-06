/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BT_OSISHTML_H
#define BT_OSISHTML_H

// Sword includes:
#include <osishtmlhref.h>
#include <swbuf.h>
#include <swmodule.h>

namespace Filters {

/** BibleTime's OSIS to HTMl filter.
* This filter works on OSIS tags and outputs HTML in the structure supported by BibleTime.
*/

class BT_OSISHTML : public sword::OSISHTMLHREF {

    protected:

        class BT_UserData : public sword::OSISHTMLHREF::MyUserData {

            public:
                BT_UserData(const sword::SWModule *module, const sword::SWKey *key) : sword::OSISHTMLHREF::MyUserData(module, key) {
                    noteType = Unknown;
                    swordFootnote = 1;
                    inCrossrefNote = false;
                    entryAttributes = module->getEntryAttributes();
                }

                unsigned short int swordFootnote;
                bool inCrossrefNote;
                sword::AttributeTypeList entryAttributes;

                enum NoteType {
                    Unknown,
                    Alternative,
                    CrossReference,
                    Footnote,
                    StrongsMarkup
                } noteType;

                struct {
                    sword::SWBuf who;
                }

                quote;
        };

        virtual sword::BasicFilterUserData *createUserData(const sword::SWModule* module, const sword::SWKey* key) {
            return new BT_UserData(module, key);
        }

    public:
        BT_OSISHTML();
        virtual bool handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData);
    private:
        void renderReference(const char *osisRef, sword::SWBuf &buf, sword::SWModule *myModule, BT_UserData *myUserData);
};

} //end of Filters namespace

#endif
