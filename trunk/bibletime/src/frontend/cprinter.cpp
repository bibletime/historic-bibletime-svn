/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "frontend/cprinter.h"

#include <QPrintDialog>
#include <QPrinter>
#include <QWebFrame>
#include <QWebPage>
#include "backend/keys/cswordversekey.h"
#include "backend/managers/cdisplaytemplatemgr.h"
#include "util/cpointers.h"


namespace Printing {

CPrinter::CPrinter(QObject*, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
        :	QObject(0),
        CDisplayRendering(displayOptions, filterOptions),
        m_htmlPage(new QWebPage()) {
    m_htmlPage->setParent(this);

    //override the filteroptions set in the c-tor of CDisplayRendering
    m_filterOptions.footnotes = false;
    m_filterOptions.scriptureReferences = false;
    m_filterOptions.strongNumbers = false;
    m_filterOptions.morphTags = false;
    m_filterOptions.headings = false;
}

CPrinter::~CPrinter() {
    delete m_htmlPage;
    m_htmlPage = 0;
}

void CPrinter::printKeyTree( KeyTree& tree ) {
    m_htmlPage->mainFrame()->setHtml(renderKeyTree(tree));

    QPrinter printer;
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        m_htmlPage->mainFrame()->print(&printer);
    }
}

const QString CPrinter::entryLink(const KeyTreeItem& item, CSwordModuleInfo* module) {
    Q_ASSERT(module);
    if (module->type() == CSwordModuleInfo::Bible) {
        CSwordVerseKey vk(module);
        vk.key(item.key());
        switch (item.settings().keyRenderingFace) {
            case KeyTreeItem::Settings::CompleteShort:
                return QString::fromUtf8(vk.getShortText());

            case KeyTreeItem::Settings::CompleteLong:
                return vk.key();

            case KeyTreeItem::Settings::NoKey:
                return QString::null;

            case KeyTreeItem::Settings::SimpleKey: //fall through
            default:
                return QString::number(vk.Verse());
        }
    }
    return item.key();
}

const QString CPrinter::renderEntry( const KeyTreeItem& i, CSwordKey* ) {
    const CPrinter::KeyTreeItem* printItem = dynamic_cast<const CPrinter::KeyTreeItem*>(&i);
    Q_ASSERT(printItem);

    if (printItem && printItem->hasAlternativeContent()) {
        QString ret = QString::fromLatin1("<div class=\"entry\"><div class=\"rangeheading\">%1</div>").arg(printItem->getAlternativeContent());

        if (!i.childList()->isEmpty()) {
            KeyTree const * tree = i.childList();

            foreach ( KeyTreeItem* c, (*tree)) {
                ret.append( CDisplayRendering::renderEntry( *c ) );
            }
        }

        ret.append("</div>");
        return ret;
    }
    return CDisplayRendering::renderEntry(i);
}

const QString CPrinter::finishText(const QString& text, KeyTree& tree) {
    QList<CSwordModuleInfo*> modules = collectModules(&tree);
    Q_ASSERT(modules.count() > 0);

    const CLanguageMgr::Language* const lang = modules.first()->language();
    Q_ASSERT(lang);

    CDisplayTemplateMgr::Settings settings;
    //settings.modules = modules;
    settings.pageCSS_ID = "printer";
    settings.langAbbrev = ( lang && (modules.count() == 1) && lang->isValid() ) ? lang->abbrev() : "unknown";

    //the previous version gave compiler error for some strange reason
    //(well, I don't like ?: anyway, let alone nested)
    if (modules.count() != 1) {
        settings.pageDirection = QString::null;
    }
    else {
        settings.pageDirection = ( modules.first()->textDirection() == CSwordModuleInfo::LeftToRight ) ? "ltr" : "rtl";
    }

    CDisplayTemplateMgr *tMgr = CDisplayTemplateMgr::instance();
    return tMgr->fillTemplate(CBTConfig::get(CBTConfig::displayStyle), text, settings);
}

} //end of namespace
