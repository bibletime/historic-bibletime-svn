/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CDISPLAYTEMPLATEMGR_H
#define CDISPLAYTEMPLATEMGR_H

#include <QMap>
#include <QString>
#include <QStringList>


class CSwordModuleInfo;

/**
  Manages the display templates used in the filters and display classes.
  \note This is a singleton.
*/
class CDisplayTemplateMgr {
    public: /* Types: */
        /**
          Settings which are used to fill the content into the template.
        */
        struct Settings {
            Settings() : pageDirection("ltr") {}

            /** The list of modules */
            QList<const CSwordModuleInfo*> modules;

            /** The title which is used for the new processed HTML page */
            QString title;

            /** The language for the HTML page. */
            QString langAbbrev;

            /** The language direction for the HTML page. */
            QString pageDirection;

            /** The CSS ID which is used in the content part of the page */
            QString pageCSS_ID;
        };

    public: /* Methods: */

        /**
          \returns the list of available templates.
        */
        inline const QStringList availableTemplates() const {
            return m_templateMap.keys();
        }

        /**
          \brief Fills the template.

          Fills rendered content into the template given by the name.

          \param name The name of the template to fill.
          \param content The content which should be filled into the template.
          \param settings The settings which are used to process the templating
                          process.

          \returns The full HTML template HTML code including the CSS data.
        */
        const QString fillTemplate( const QString& name, const QString& content, Settings& settings);

        /**
          \returns the name of the default template.
        */
        inline static const char *defaultTemplate() { return "Blue.tmpl"; }

        /**
          \returns the singleton instance, creating it if one does not exist.
        */
        static inline CDisplayTemplateMgr *instance() {
            if (m_instance == 0) m_instance = new CDisplayTemplateMgr();
            return m_instance;
        };

        /** Destroys the singleton instance, if one exists. */
        static inline void destroyInstance() {
            delete m_instance;
            m_instance = 0;
        }

    protected: /* Methods: */

        /** Preloads templates from disk. */
        CDisplayTemplateMgr();

    private: /* Methods: */
        /** Preloads a single template from disk: */
        void loadTemplate(const QString &filename);

    private: /* Fields: */
        QMap<QString, QString> m_templateMap;
        static CDisplayTemplateMgr *m_instance;
};

#endif
