/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2008 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef CDISPLAYTEMPLATEMGR_H
#define CDISPLAYTEMPLATEMGR_H

//BibleTime include
class CSwordModuleInfo;

//Qt includes
#include <QMap>
#include <QString>
#include <QStringList>

/**
 * Manages the display templates used in the filters and display classes.
 * @author The BibleTime team
*/

class CDisplayTemplateMgr {

    public:
        /** Settings which are used to fill the content into the template.
        */

        struct Settings {
            /** Constructor. Constructs the new settings object. The default values are empty.
             */
            Settings() {
                title = QString::null;
                langAbbrev = QString::null;
                pageCSS_ID = QString::null;
                pageDirection = QString("ltr");
            };

            QList<CSwordModuleInfo*> modules; /**< the list of modules */
            QString title; /**< the title which is used for the new processed HTML page */
            QString langAbbrev; /**< the language for the HTML page. */
            QString pageDirection; /**< the language for the HTML page. */
            QString pageCSS_ID; /**< the CSS ID which is used in the content part of the page */
        };

        /** Available templates.
        * @return The list of templates, which are available.
        */
        inline const QStringList availableTemplates();
        /** Fill template. Fill rendered content into the template given by the name.
        * @param name The name of the template
        * @param content The content which should be filled into the template
        * @param settings The settings which are used to process the templating process
        * @return The full HTML template HTML code including the CSS data.
        */
        const QString fillTemplate( const QString& name, const QString& content, Settings& settings);
        /** Default template.
        * @return The i18n'ed name of the default template
        */
        inline static const QString defaultTemplate();

    protected:
        friend class CPointers;
        /** Display template manager constructor. Protected to just allow CPointers to create objects. */
        CDisplayTemplateMgr();
        /** Destructor. */
        ~CDisplayTemplateMgr();
        /** Does the actual work of loading templates from disk */
        void loadTemplates();

    private:
        QMap<QString, QString> m_templateMap;
};

inline const QString CDisplayTemplateMgr::defaultTemplate() {
    return QString("Blue.tmpl");
}

/**
 * CDisplayTemplateMgr::availableTemplates()
 */
inline const QStringList CDisplayTemplateMgr::availableTemplates() {
    return m_templateMap.keys();
}



#endif
