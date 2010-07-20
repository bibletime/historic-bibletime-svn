/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CSWORDLEXICONMODULEINFO_H
#define CSWORDLEXICONMODULEINFO_H

#include "backend/drivers/cswordmoduleinfo.h"

#include <QStringList>


/**
  The implementation of CModuleInfo for the Sword lexiccons and citionaries.
*/
class CSwordLexiconModuleInfo: public CSwordModuleInfo {
        Q_OBJECT

    public: /* Methods: */
        inline CSwordLexiconModuleInfo(sword::SWModule *module,
                                       CSwordBackend * const backend)
                : CSwordModuleInfo(module, backend, Lexicon)
        {
            initEntries();
        }

        inline CSwordLexiconModuleInfo(const CSwordLexiconModuleInfo &copy)
            : CSwordModuleInfo(copy), m_entries(copy.m_entries) {}

        /* Reimplementation of CSwordModuleInfo::clone(). */
        virtual inline CSwordModuleInfo *clone() const {
            return new CSwordLexiconModuleInfo(*this);
        }

        /**
          This method returns the entries of the modules represented by this
          object. If this function is called for the first time the list is load
          from disk and stored in a list which cahes it. If the function is
          called again, the cached list is returned so we have a major speed
          improvement.
          \returns the list of lexicon entries in the module.
        */
        inline const QStringList &entries() const { return m_entries; }

        /**
          Jumps to the closest entry in the module.
        */
        virtual inline bool snap() const {
            return module()->getRawEntry();
        }

    private: /* Methods: */
        void initEntries();

    private: /* Fields: */
        /**
          This is the list which caches the entres of the module.
        */
        QStringList m_entries;
};

#endif
