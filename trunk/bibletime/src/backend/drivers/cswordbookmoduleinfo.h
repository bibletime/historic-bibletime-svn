/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CSWORDGENBOOKMODULEINFO_H
#define CSWORDGENBOOKMODULEINFO_H

#include "backend/drivers/cswordmoduleinfo.h"

// Sword includes:
#include <treekeyidx.h>


/**
  \brief Class for generic book support
*/
class CSwordBookModuleInfo: public CSwordModuleInfo {
        Q_OBJECT

    public: /* Methods: */
        /**
          \param module The module which belongs to this object
          \param backend The parent backend for this book module.
        */
        CSwordBookModuleInfo(sword::SWModule *module,
                             CSwordBackend * const usedBackend);

        inline CSwordBookModuleInfo(const CSwordBookModuleInfo &copy)
            : CSwordModuleInfo(copy), m_depth(copy.m_depth) {}

        /* Reimplementation of CSwordModuleInfo::clone(). */
        virtual inline CSwordModuleInfo *clone() const {
            return new CSwordBookModuleInfo(*this);
        }

        /**
          \returns the maximal depth of sections and subsections.
        */
        inline int depth() const { return m_depth; }

        /**
          \returns A treekey filled with the structure of this module. Don't
                   delete the returned key because it's casted from the module
                   object.
        */
        sword::TreeKeyIdx *tree() const;

    private: /* Methods: */
        /**
        * A recursive helper function to help computng the module depth!
        */
        void computeDepth(sword::TreeKeyIdx *key, int level = 0);

    private: /* Fields: */
        int m_depth;
};

#endif
