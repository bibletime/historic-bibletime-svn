/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "util/tool.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QRegExp>
#include <QTextStream>
#include <QWidget>
#include "backend/drivers/cswordmoduleinfo.h"
#include "backend/managers/cswordbackend.h"
#include "util/cresmgr.h"
#include "util/directory.h"
#include "util/dialogutil.h"


/** Creates the file filename and put text into the file.
 */
bool util::tool::savePlainFile( const QString& filename, const QString& text, const bool& forceOverwrite, QTextCodec* fileCodec) {
    QFile saveFile(filename);
    bool ret;

    if (saveFile.exists()) {
        if (!forceOverwrite && util::showQuestion(0, QObject::tr("Overwrite File?"),
                QString::fromLatin1("<qt><b>%1</b><br/>%2</qt>")
                .arg( QObject::tr("The file already exists.") )
                .arg( QObject::tr("Do you want to overwrite it?")),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No) == QMessageBox::No
           ) {
            return false;
        }
        else { //either the user chose yes or forceOverwrite is set
            saveFile.remove();
        }
    }

    if ( saveFile.open(QIODevice::ReadWrite) ) {
        QTextStream textstream( &saveFile );
        textstream.setCodec(fileCodec);
        textstream << text;
        saveFile.close();
        ret = true;
    }
    else {
        QMessageBox::critical(0, QObject::tr("Error"),
                              QString::fromLatin1("<qt>%1<br/><b>%2</b></qt>")
                              .arg( QObject::tr("The file couldn't be saved.") )
                              .arg( QObject::tr("Please check permissions etc.")));
        saveFile.close();
        ret = false;
    }
    return ret;
}


QIcon util::tool::getIconForModule(const CSwordModuleInfo *module) {
    namespace DU = util::directory;
    return DU::getIcon(getIconNameForModule(module));
}

QString util::tool::getIconNameForModule(const CSwordModuleInfo *module) {
    //qDebug() << "util::tool::getIconNameForModule";
    if (!module) return CResMgr::modules::book::icon_locked;

    if (module->category() == CSwordModuleInfo::Cult) {
        return "stop.svg";
    }

    switch (module->type()) {
        case CSwordModuleInfo::Bible:
            if (module->isLocked())
                return CResMgr::modules::bible::icon_locked;
            else
                return CResMgr::modules::bible::icon_unlocked;
            break;

        case CSwordModuleInfo::Lexicon:
            if (module->isLocked())
                return CResMgr::modules::lexicon::icon_locked;
            else
                return CResMgr::modules::lexicon::icon_unlocked;
            break;

        case CSwordModuleInfo::Commentary:
            if (module->isLocked())
                return CResMgr::modules::commentary::icon_locked;
            else
                return CResMgr::modules::commentary::icon_unlocked;
            break;

        case CSwordModuleInfo::GenericBook:
            if (module->isLocked())
                return CResMgr::modules::book::icon_locked;
            else
                return CResMgr::modules::book::icon_unlocked;
            break;

        case CSwordModuleInfo::Unknown: //fallback
        default:
            if (module->isLocked())
                return CResMgr::modules::book::icon_locked;
            else
                return CResMgr::modules::book::icon_unlocked;
            break;
    }
    return CResMgr::modules::book::icon_unlocked;
}

QLabel* util::tool::explanationLabel(QWidget *parent, const QString &heading, const QString &text) {
    QLabel *label = new QLabel(parent);
    initExplanationLabel(label, heading, text);
    return label;
}

void util::tool::initExplanationLabel(QLabel *label, const QString &heading, const QString &text) {
    QString labelText;
    if (!heading.isEmpty()) {
        labelText += "<b>";
        labelText += heading;
        labelText += "</b>";
    }
    if (!heading.isEmpty() && !text.isEmpty()) {
        labelText += "<span style=\"white-space:pre\">  -  </span>";
    }
    if (!text.isEmpty()) {
        labelText += "<small>";
        labelText += text;
        labelText += "</small>";
    }
    label->setText(labelText);
    label->setWordWrap(true);
    label->setMargin(1);
    label->setFrameStyle(QFrame::Box | QFrame::Sunken);
}

/** No descriptions */
bool util::tool::inHTMLTag(int pos, QString & text) {
    int i1 = text.lastIndexOf("<", pos);
    int i2 = text.lastIndexOf(">", pos);
    int i3 = text.indexOf(">", pos);
    int i4 = text.indexOf("<", pos);


    // if ((i1>0) && (i2==-1))  //we're in th first html tag
    //  i2=i1; // not ncessary, just for explanation

    if ((i3 > 0) && (i4 == -1))  //we're in the last html tag
        i4 = i3 + 1;

    //  qWarning("%d > %d && %d < %d",i1,i2,i3,i4);

    if ( (i1 > i2) && (i3 < i4) )
        return true; //yes, we're in a tag

    return false;
}

QString util::tool::remoteModuleToolTip(const CSwordModuleInfo &module,
                                        const QString &localVer)
{
    QString text = "<p style='white-space:pre'><b>";
    text += module.name();
    text += "</b> ";

    if (module.category() == CSwordModuleInfo::Cult) {
        text += "<small><b>";
        text += QObject::tr("Take care, this work contains cult / questionable "
                            "material!");
        text += "</b></small><br/>";
    }

    text += "<small>(";
    text += module.config(CSwordModuleInfo::Description);
    text += ")</small><hr/>";

    if (module.isEncrypted()) {
        text += QObject::tr("Encrypted - needs unlock key");
        text += "<br/>";
    }

    if (!localVer.isEmpty()) {
        text += "<b>";
        text += QObject::tr("Updated version available!");
        text += "</b><br/>";
    }

    if (module.hasVersion()) {
        text += QObject::tr("Version");
        text += ": ";
        text += module.config(CSwordModuleInfo::ModuleVersion);
    }

    // if installed already
    if (!localVer.isEmpty()) {
        text += "  ";
        text += QObject::tr("Installed version");
        text += ": ";
        text += localVer;
    }
    text += "<br/><small>(";
    text += QObject::tr("Double click for more information");
    text += ")</small></p>";

    return text;
}


int util::tool::mWidth(const QWidget* widget, int m) {
    if (widget) return widget->fontMetrics().width(QString(m, 'M'));
    return QApplication::fontMetrics().width(QString(m, 'M'));
}
