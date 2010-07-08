/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CSWORDMODULEINFO_H
#define CSWORDMODULEINFO_H

#include "backend/managers/clanguagemgr.h"

#include <QList>
#include <QMetaType>
#include <QString>

// Sword includes:
#include <listkey.h>
#include <swdisp.h>
#include <swmodule.h>
#include <swsearchable.h>
#include <swversion.h>


class CSwordBackend;
class CSwordKey;

namespace Rendering {

class CEntryDisplay;
}

/**
 * Base class for Sword modules.
 * This is the base class for all Sword modules. Every class handling a special Sword module type
 * does inherit from this class.
 *
 * @author The BibleTime team
 * @version $Id: cswordmoduleinfo.h,v 1.83 2007/02/04 23:12:32 joachim Exp $
 */

class CSwordModuleInfo: public QObject {
        Q_OBJECT

    public:
        /**
         * These are the options which could be supported by modules and by this backend.
         * It's used in @ref CSwordBackend::setOption.
         */
        enum FilterTypes {
            footnotes, /**< Footnotes embedded in the module's text */
            strongNumbers, /**< strong numbers, usually in the text for the info display */
            headings, /**< additional section headings */
            morphTags, /**< morphology */
            lemmas, /**< lemma tags */
            hebrewPoints,/**< Hebrew vowel points */
            hebrewCantillation, /**<Hewbrew caantillation points */
            greekAccents, /**< Greek accents may be switched on and off */
            scriptureReferences, /**< scripture references may be switched on and off, just makes sense in Bibles */
            redLetterWords, /**< Jesus words in red, color is template specific */
            textualVariants, /**< variants */
            morphSegmentation, /**< morph word segmentation, supported by OSIS */
            filterTypesMIN = footnotes, /**< first entry of this enum */
            filterTypesMAX = morphSegmentation /**< last item in this enum */
            //     transliteration /* The following are handled in a special way */
        };
        /** The text direction of a module */
        enum TextDirection { /* The text direction of the modules's text */
            LeftToRight, /**< Left to right text direction, the default setting */
            RightToLeft /**< Right to left text directin, e.g. for hebrew */
        };
        /** The module type.
        */
        enum ModuleType {
            Bible, /**< Bible module */
            Commentary, /**< Commentary module */
            Lexicon, /**< Lexicon module */
            GenericBook, /**< Generic book module */
            Unknown /**< Fall back type for unknown modules */
        };
        /**
         * This enum is used to give
         * back an error code after unlocking the module
         * BibleTime stores the unlock key not in the module's config file but in BibleTime's
         * configuration file.
         */
        enum UnlockErrorCode {
            noError, /**< No error occured, everything worked ok. The key was written to the BibleTime config */
            wrongUnlockKey, /**< The wrong key was used. Module is not unlocked */
            notLocked, /**< The module was not locked so it can't be unlocked */
            noPermission /**< The key was not written to config because we have no permissions */
        };
        enum ConfigEntry {
            AboutInformation, /**< The about information of a module which is stored in the config file*/
            AbsoluteDataPath, /**< The absolute data path stored in the config object */
            CipherKey, /**< The cipher key which was used to unlock the module. Not necessarily set.*/
            DataPath, /**< The relative path. See AbsoluteDataPath*/
            Description, /**< The module description stored in the config file */
            ModuleVersion, /**< The module's version.*/
            MinimumSwordVersion, /**< The required Sword Version of this module. Otherwise some things may not work (compression etc.).*/
            TextDir, /**< The text direction */
            DisplayLevel, /**< Mostly used for books. Gives the level which should contain the connected entries.*/
            GlossaryFrom, /**< lamguage from which the Glosaary tramslates */
            GlossaryTo, /**< lamguages to which the glossary maps to */
            DistributionLicense,
            DistributionSource,
            DistributionNotes,
            TextSource,
            CopyrightNotes,
            CopyrightHolder,
            CopyrightDate,
            CopyrightContactName,
            CopyrightContactAddress,
            CopyrightContactEmail,
            Markup  /**< The markup of this module */
        };
        enum Feature {
            //StrongsNumbers, /**< Use for Bibles which have embedded strong numbers */ BT does not use this as a user option
            GreekDef,
            HebrewDef,
            GreekParse,
            HebrewParse,
            featureMin = GreekDef,
            featureMax = HebrewParse
        };
        enum Category {
            UnknownCategory = 0x0,  /**< Unknown or unset category. */
            NoCategory      = 0x0,
            Bibles          = 0x01,
            Commentaries    = 0x02,
            Books           = 0x04,
            Lexicons        = 0x08,
            Glossary        = 0x10,
            DailyDevotional = 0x20,
            Images          = 0x40,
            Cult            = 0x80, /**< Cult / sect / questionable module. */
            AllCategories   = 0xff
        };
        Q_DECLARE_FLAGS(Categories, Category);

        /**
        * Returns the base directory for search indices
        */
        static QString getGlobalBaseIndexLocation();

        /**
          Removes the search index for this module (rm -rf).
        */
        void deleteIndex();

        /**
          Removes search index for a module, even if the module is not there any more.
          \param[in] name name of the module.
        */
        static void deleteIndexForModule(const QString &name);


        /**
        * Returns the config entry which is pecified by the parameter.
        */
        QString config( const CSwordModuleInfo::ConfigEntry entry ) const;

        CSwordModuleInfo(sword::SWModule *module,
                         CSwordBackend * const = 0,
                         ModuleType type = Unknown);

        CSwordModuleInfo(const CSwordModuleInfo &copy);

        virtual CSwordModuleInfo *clone() const = 0;

        virtual inline ~CSwordModuleInfo() {
            // m_module is deleted by the backend
            /// \todo is this needed?
            m_searchResult.ClearList();
        }


        /**
        * Returns the module object so all objects can access the original Sword module.
        */
        inline sword::SWModule *module() const {
            return m_module;
        }

        /**
        * Sets the unlock key of the modules and writes the key into the cofig file.
        * @return True if the unlock process was succesful, if the key was wrong, or if the config file was write protected return false.
        */
        bool unlock( const QString& unlockKey );
        /**
        * Returns the display object for this module. Normally every module should have a Display object.
        * Please don't use module()->Display() because this function does return the Sword display and does
        * render the text, too.
        * This function performs some casts to return the correct display. If it returns 0 there's no valid
        * display object.
        */
        Rendering::CEntryDisplay* getDisplay() const;
        /**
        * This function does return true if the data files of the module are encrypted by the module author
        * (the on who made the module) no matter if it's locked or not.
        * @return True if this module is encryped
        */
        bool isEncrypted() const;
        /**
        * This function returns true if this module is locked (encrypted + correct cipher key),
        * otherwise return false.
        * @return True if this module is locked, i.e. encrypted but without a key set
        */
        bool isLocked() const;


        /**
          This function makes an estimate if a module was properly unlocked. It
          returns true if the first entry of the module is not empty and
          contains only printable characters (for the first 100 chars or so). If
          that is the case, we can safely assume that a) the module was properly
          unlocked and b) no buffer overflows will occur, which can happen when
          Sword filters process garbage text which was not properly decrypted.
        */
        bool unlockKeyIsValid() const;

        /**
          \retval true if this module has a version number
          \retval false if it doesn't have a version number
        */
        inline bool hasVersion() const {
            return m_cachedHasVersion;
        }

        /**
          \returns true if the module's index has been built.
        */
        bool hasIndex();

        /**
          \returns the path to this module's index base dir
        */
        QString getModuleBaseIndexLocation() const;

        /**
          \returns the path to this module's standard index
        */
        QString getModuleStandardIndexLocation() const;

        /**
          Builds a search index for this module
        */
        void buildIndex();

        /**
          \returns index size
        */
        unsigned long indexSize() const;

        /**
          This function uses CLucene to perform and index based search. It also
          overwrites the variable containing the last search result.
          \returns whether something was found
        */
        bool searchIndexed(const QString &searchedText, sword::ListKey &scope);

        /**
        * Returns the last search result for this module.
        * The last result is cleared by @ref search
        */
        sword::ListKey &searchResult(const sword::ListKey *newResult = 0);

        /**
          \returns the type of the module.
        */
        inline ModuleType type() const {
            return m_type;
        }

        /**
        * Returns the required Sword version for this module.
        * Returns -1 if no special Sword version is required.
        */
        sword::SWVersion minimumSwordVersion() const;

        /**
          \note The Sword library takes care of the duplicate names: _n is added
                after each duplicate.
          \returns The name of this module.
        */
        inline const QString &name() const {
            return m_cachedName;
        }

        /**
        * Snaps to the closest entry in the module if the current key is
        * not present in the data files.
        */
        virtual bool snap() {
            return false;
        }

        /**
          \returns whether the module supports the feature given as parameter.
        */
        bool has(const CSwordModuleInfo::Feature) const;

        bool has(const CSwordModuleInfo::FilterTypes ) const;

        /**
          \returns the text direction of the module's text.
        */
        CSwordModuleInfo::TextDirection textDirection() const;

        /**
          Writes the new text at the given position into the module. This does
          only work for writabe modules.
        */
        void write(CSwordKey *key, const QString &newText);

        /**
          Deletes the current entry and removes it from the module.
        */
        bool deleteEntry(CSwordKey * const key);

        /**
          \returns the language of the module.
        */
        inline const CLanguageMgr::Language *language() const {
            return m_cachedLanguage;
        }

        /**
          \returns whether this module may be written to.
        */
        inline virtual bool isWritable() const {
            return false;
        }

        /**
        * Returns true if this module is hidden (not to be shown with other modules in certain views).
        */
        inline bool isHidden() const {
            return m_hidden;
        }

        /**
          Shows or hides the module.
          \param hide Whether the module should be hidden.
          \returns whether the hidden state was changed.
        */
        bool setHidden(bool hide);

        /**
          \returns the category of this module.
        */
        inline CSwordModuleInfo::Category category() const {
            return m_cachedCategory;
        }

        /**
        * The about text which belongs to this module.
        */
        QString aboutText() const;
        /**
        * Returns true if this module is Unicode encoded. False if the charset is iso8859-1.
        * Protected because it should not be used outside of the CSword*ModuleInfo classes.
        */
        inline bool isUnicode() const {
            return m_module->isUnicode();
        }

        /**
          Returns an icon for the given module.
          \param[in] module The module whose icon to return.
        */
        static QIcon moduleIcon(const CSwordModuleInfo *module);

        /**
          Returns an icon for the category of given module.
          \param[in] module The module whose category icon to return.
        */
        static QIcon categoryIcon(const CSwordModuleInfo::Category &category);

        /**
          Returns a translated name for the given category.
          \param[in] module The category whose translated name to return.
        */
        static QString categoryName(const CSwordModuleInfo::Category &category);

    public slots:
        inline void cancelIndexing() {
            m_cancelIndexing = true;
        }

    protected:
        friend class CSwordBackend;

        inline CSwordBackend* backend() const {
            return m_backend;
        }

        inline void backend( CSwordBackend* newBackend ) {
            if (newBackend) {
                m_backend = newBackend;
            }
        }

        QString getSimpleConfigEntry(const QString& name) const;
        QString getFormattedConfigEntry(const QString& name) const;

    private: /* Methods: */
        /**
          Initializes CSwordModuleInfo::m_cachedCategory.
          \pre m_module must be set
        */
        void initCachedCategory();

        /**
          Initializes CSwordModuleInfo::m_cachedLanguage.
          \pre CSwordModuleInfo::m_module must be set
          \pre CSwordModuleInfo::m_cachedLanguage must be set
        */
        void initCachedLanguage();

    signals:
        void hasIndexChanged(bool hasIndex);
        void hiddenChanged(bool hidden);
        void unlockedChanged(bool unlocked);
        void indexingFinished();
        void indexingProgress(int);

    private:
        sword::SWModule *m_module;
        CSwordBackend *m_backend;
        sword::ListKey m_searchResult;
        ModuleType m_type;
        bool m_hidden;
        bool m_cancelIndexing;

        // Cached data:
        const QString m_cachedName;
        CSwordModuleInfo::Category m_cachedCategory;
        const CLanguageMgr::Language *m_cachedLanguage;
        bool m_cachedHasVersion;
};

Q_DECLARE_METATYPE(CSwordModuleInfo::Category);
Q_DECLARE_OPERATORS_FOR_FLAGS(CSwordModuleInfo::Categories)

#endif
