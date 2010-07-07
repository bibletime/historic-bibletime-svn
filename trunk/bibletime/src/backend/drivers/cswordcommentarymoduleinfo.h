/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CSWORDCOMMENTARYMODULEINFO_H
#define CSWORDCOMMENTARYMODULEINFO_H

#include "backend/drivers/cswordbiblemoduleinfo.h"


/**
  \brief Commentary module implementation.

  This CSwordModule implementation provides access to Sword's commentary modules.
*/
class CSwordCommentaryModuleInfo: public CSwordBibleModuleInfo {
        Q_OBJECT

    public: /* Methods: */
        inline CSwordCommentaryModuleInfo(sword::SWModule *module,
                                          CSwordBackend * const usedBackend)
                : CSwordBibleModuleInfo(module, usedBackend,
                                        CSwordModuleInfo::Commentary) {}

        /* Reimplementation of CSwordModuleInfo::clone(). */
        virtual inline CSwordModuleInfo* clone() const {
            return new CSwordCommentaryModuleInfo(*this);
        }

        /* Reimplementation of CSwordModuleInfo::isWritable(). */
        virtual bool isWritable() const;
};

#endif
