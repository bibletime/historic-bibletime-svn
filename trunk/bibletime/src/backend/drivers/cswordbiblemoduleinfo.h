/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CSWORDBIBLEMODULEINFO_H
#define CSWORDBIBLEMODULEINFO_H

#include "backend/drivers/cswordmoduleinfo.h"

#include <QStringList>
#include "backend/keys/cswordversekey.h"


/**
  \brief Implementation for Sword Bibles.

  This is the CModuleInfo imlementation for Bible modules managed by Sword.
*/
class CSwordBibleModuleInfo: public CSwordModuleInfo {
        Q_OBJECT

    public: /* Types: */
        enum Testament {
            OldTestament = 1,
            NewTestament = 2
        };

    public: /* Methods: */
        CSwordBibleModuleInfo(sword::SWModule *module, CSwordBackend * const,
                              ModuleType type = Bible);
        CSwordBibleModuleInfo(const CSwordBibleModuleInfo &copy);

        /* Reimplementation of CSwordModuleInfo::clone(). */
        virtual inline CSwordModuleInfo *clone() const {
            return new CSwordBibleModuleInfo(*this);
        }

        inline ~CSwordBibleModuleInfo() {
            delete m_bookList;
        }

        /**
          \returns the number of avalable verses for the given chapter and book.
          \param book The number book we should use
          \param chapter The chapter we should use
        */
        unsigned int verseCount(const unsigned int book,
                                const unsigned int chapter) const;

        /**
          \returns the number of avalable verses for the given chapter and book.
          \param book The name of the book we use
          \param chapter The number of the chapter we use
        */
        unsigned int verseCount(const QString &book,
                                const unsigned int chapter) const;

        /**
          \returns the number of available chapters in the given book.
        */
        unsigned int chapterCount(const unsigned int book) const;

        /**
          \returns the number of available chapters in the given book.
        */
        unsigned int chapterCount(const QString &book) const;

        /**
          \returns a QStringList containing the books available in this module.
        */
        QStringList *books() const;

        /**
          \returns the index of the book given by its name.
          \retval 0 if a book with the given name was not found.
        */
        unsigned int bookNumber(const QString &book) const;

        /**
          \returns whether this module has the text of desired type of testament
        */
        bool hasTestament(CSwordBibleModuleInfo::Testament type) const {
            return type == OldTestament ? m_hasOT : m_hasNT;
        }

        /**
          \returns the key which represents the lower bound of this module.
        */
        inline const CSwordVerseKey &lowerBound() const {
            return m_lowerBound;
        }

        /**
          \returns the key which represents the upper bound of this module.
        */
        inline const CSwordVerseKey &upperBound() const {
            return m_upperBound;
        }

    private: /* Methods: */
        void initBounds();

    private:
        CSwordVerseKey m_lowerBound;
        CSwordVerseKey m_upperBound;

        mutable QStringList *m_bookList; //This booklist is cached
        mutable QString m_cachedLocale;
        bool m_hasOT;
        bool m_hasNT;
};

#endif
