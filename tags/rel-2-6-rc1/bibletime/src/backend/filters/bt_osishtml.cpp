/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#include "backend/filters/bt_osishtml.h"

#include <QString>
#include "backend/config/cbtconfig.h"
#include "backend/drivers/cswordmoduleinfo.h"
#include "backend/managers/clanguagemgr.h"
#include "backend/managers/referencemanager.h"
#include "util/cpointers.h"

// Sword includes:
#include <swbuf.h>
#include <swmodule.h>
#include <utilxml.h>


Filters::BT_OSISHTML::BT_OSISHTML() : sword::OSISHTMLHREF() {
    setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes

    addTokenSubstitute("inscription", "<span class=\"inscription\">");
    addTokenSubstitute("/inscription", "</span>");

    addTokenSubstitute("mentioned", "<span class=\"mentioned\">");
    addTokenSubstitute("/mentioned", "</span>");

//	addTokenSubstitute("divineName", "<span class=\"name\"><span class=\"divine\">");
//	addTokenSubstitute("/divineName", "</span></span>");

    /// \todo Move that down to the real tag handling, segs without the type morph would generate incorrect markup, as the end span is always inserted
//	addTokenSubstitute("seg type=\"morph\"", "<span class=\"morphSegmentation\">");
//	addTokenSubstitute("/seg", "</span>");

    // OSIS tables
    addTokenSubstitute("table", "<table>");
    addTokenSubstitute("/table", "</table>");
    addTokenSubstitute("row", "<tr>");
    addTokenSubstitute("/row", "</tr>");
    addTokenSubstitute("cell", "<td>");
    addTokenSubstitute("/cell", "</td>");

}

bool Filters::BT_OSISHTML::handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData) {
    // manually process if it wasn't a simple substitution

    if (!substituteToken(buf, token)) {
        BT_UserData* myUserData = dynamic_cast<BT_UserData*>(userData);
        sword::SWModule* myModule = const_cast<sword::SWModule*>(myUserData->module); //hack

        sword::XMLTag tag(token);
        //     qWarning("found %s", token);
        const bool osisQToTick = ((!userData->module->getConfigEntry("OSISqToTick")) || (strcmp(userData->module->getConfigEntry("OSISqToTick"), "false")));

        if (!tag.getName()) {
            return false;
        }

        // <div> tag
        if (!strcmp(tag.getName(), "div")) {
            //handle intro

            if ((!tag.isEmpty()) && (!tag.isEndTag())) { //start tag
                sword::SWBuf type( tag.getAttribute("type") );

                if (type == "introduction") {
                    buf.append("<div class=\"introduction\">");
                }
                else if (type == "chapter") {
                    buf.append("<div class=\"chapter\" />"); //don't open a div here, that would lead to a broken XML structure
                }
                else {
                    buf.append("<div>");
                }
            }
            else if (tag.isEndTag()) { //end tag
                buf.append("</div>");
            }
        }
        else if (!strcmp(tag.getName(), "w")) {
            if ((!tag.isEmpty()) && (!tag.isEndTag())) { //start tag
                const char *attrib;
                const char *val;

                sword::XMLTag outTag("span");
                sword::SWBuf attrValue;

                if ((attrib = tag.getAttribute("xlit"))) {
                    val = strchr(attrib, ':');
                    val = (val) ? (val + 1) : attrib;
                    outTag.setAttribute("xlit", val);
                }

                if ((attrib = tag.getAttribute("gloss"))) {
                    val = strchr(attrib, ':');
                    val = (val) ? (val + 1) : attrib;
                    outTag.setAttribute("gloss", val);
                }

                if ((attrib = tag.getAttribute("lemma"))) {
                    char splitChar = '|';
                    const int countSplit1 = tag.getAttributePartCount("lemma", '|');
                    const int countSplit2 = tag.getAttributePartCount("lemma", ' '); /// \todo not allowed, remove soon
                    int count = 0;

                    if (countSplit1 > countSplit2) { //| split char
                        splitChar = '|'; /// \todo not allowed, remove soon
                        count = countSplit1;
                    }
                    else {
                        splitChar = ' ';
                        count = countSplit2;
                    }

                    int i = (count > 1) ? 0 : -1;  // -1 for whole value cuz it's faster, but does the same thing as 0
                    attrValue = "";

                    do {
                        if (attrValue.length()) {
                            attrValue.append( '|' );
                        }

                        attrib = tag.getAttribute("lemma", i, splitChar);

                        if (i < 0) { // to handle our -1 condition
                            i = 0;
                        }

                        val = strchr(attrib, ':');
                        val = (val) ? (val + 1) : attrib;

                        attrValue.append(val);
                    }
                    while (++i < count);

                    if (attrValue.length()) {
                        outTag.setAttribute("lemma", attrValue.c_str());
                    }
                }

                if ((attrib = tag.getAttribute("morph"))) {
                    char splitChar = '|';
                    const int countSplit1 = tag.getAttributePartCount("morph", '|');
                    const int countSplit2 = tag.getAttributePartCount("morph", ' '); /// \todo not allowed, remove soon
                    int count = 0;

                    if (countSplit1 > countSplit2) { //| split char
                        splitChar = '|';
                        count = countSplit1;
                    }
                    else {
                        splitChar = ' ';
                        count = countSplit2;
                    }

                    int i = (count > 1) ? 0 : -1;  // -1 for whole value cuz it's faster, but does the same thing as 0

                    attrValue = "";

                    do {
                        if (attrValue.length()) {
                            attrValue.append('|');
                        }

                        attrib = tag.getAttribute("morph", i, splitChar);

                        if (i < 0) {
                            i = 0; // to handle our -1 condition
                        }

                        val = strchr(attrib, ':');

                        if (val) { //the prefix gives the modulename
                            //check the prefix
                            if (!strncmp("robinson:", attrib, 9)) { //robinson
                                attrValue.append( "Robinson:" ); //work is not the same as Sword's module name
                                attrValue.append( val + 1 );
                            }
                            //strongs is handled by BibleTime
                            /*else if (!strncmp("strongs", attrib, val-atrrib)) {
                            	attrValue.append( !strncmp(attrib, "x-", 2) ? attrib+2 : attrib );
                            }*/
                            else {
                                attrValue.append( !strncmp(attrib, "x-", 2) ? attrib + 2 : attrib );
                            }
                        }
                        else { //no prefix given
                            val = attrib;
                            const bool skipFirst = ((val[0] == 'T') && ((val[1] == 'H') || (val[1] == 'H')));
                            attrValue.append( skipFirst ? val + 1 : val );
                        }
                    }
                    while (++i < count);

                    if (attrValue.length()) {
                        outTag.setAttribute("morph", attrValue.c_str());
                    }
                }

                if ((attrib = tag.getAttribute("POS"))) {
                    val = strchr(attrib, ':');
                    val = (val) ? (val + 1) : attrib;
                    outTag.setAttribute("pos", val);
                }

                buf.append( outTag.toString() );
            }
            else if (tag.isEndTag()) { // end or empty <w> tag
                buf.append("</span>");
            }
        }

        // <note> tag
        else if (!strcmp(tag.getName(), "note")) {
            if (!tag.isEndTag()) { //start tag
                const sword::SWBuf type( tag.getAttribute("type") );

                if (type == "crossReference") { //note containing cross references
                    myUserData->inCrossrefNote = true;
                    myUserData->noteType = BT_UserData::CrossReference;

                    /*
                     * Do not count crossrefs as footnotes if they are displayed in the text. This will cause problems
                     * with footnote numbering when crossrefs are turned on/off.
                     * When accessing footnotes, crossrefs must be turned off in the filter so that they are not in the entry
                     * attributes of Sword.
                     *
                     * //myUserData->swordFootnote++; // cross refs count as notes, too
                     */

                    buf.append("<span class=\"crossreference\">");
                    sword::SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
                    sword::SWBuf footnoteBody = myUserData->entryAttributes["Footnote"][footnoteNumber]["body"];
                    buf += myModule->RenderText(footnoteBody);
                }

                /* else if (type == "explanation") {
                     }
                     */
                else if ((type == "strongsMarkup") || (type == "x-strongsMarkup")) {
                    /**
                    * leave strong's markup notes out, in the future we'll probably have
                    * different option filters to turn different note types on or off
                    */

                    myUserData->suspendTextPassThru = true;
                    myUserData->noteType = BT_UserData::StrongsMarkup;
                }

                else {
                    //           qWarning("found note in %s", myUserData->key->getShortText());
                    buf.append(" <span class=\"footnote\" note=\"");
                    buf.append(myModule->Name());
                    buf.append('/');
                    buf.append(myUserData->key->getShortText());
                    buf.append('/');
                    buf.append( QString::number(myUserData->swordFootnote++).toUtf8().constData() ); //inefficient

                    const sword::SWBuf n = tag.getAttribute("n");

                    buf.append("\">");
                    buf.append( (n.length() > 0) ? n.c_str() : "*" );
                    buf.append("</span> ");

                    myUserData->noteType = BT_UserData::Footnote;
                    myUserData->suspendTextPassThru = true;
                }
            }
            else { //if (tag.isEndTag()) {
                Q_ASSERT(myUserData->noteType != BT_UserData::Unknown);

                if (myUserData->noteType == BT_UserData::CrossReference) {
                    buf.append("</span> ");
// 					myUserData->suspendTextPassThru = false;
                    myUserData->inCrossrefNote = false;
                }

                myUserData->noteType = BT_UserData::Unknown;
                myUserData->suspendTextPassThru = false;
            }
        }
        // The <p> paragraph tag is handled by OSISHTMLHref
        else if (!strcmp(tag.getName(), "reference")) { // <reference> tag
            if (!tag.isEndTag() && !tag.isEmpty()) {

                renderReference(tag.getAttribute("osisRef"), buf, myModule, myUserData);

            }
            else if (tag.isEndTag()) {
                buf.append("</a>");
            }
            else { // empty reference marker
                // -- what should we do?  nothing for now.
            }
        }

        // <l> is handled by OSISHTMLHref
        // <title>
        else if (!strcmp(tag.getName(), "title")) {
            if (!tag.isEndTag() && !tag.isEmpty()) {
                buf.append("<div class=\"sectiontitle\">");
            }
            else if (tag.isEndTag()) {
                buf.append("</div>");
            }
            else { // empty title marker
                // what to do?  is this even valid?
                buf.append("<br/>");
            }
        }

        // <hi> highlighted text
        else if (!strcmp(tag.getName(), "hi")) {
            const sword::SWBuf type = tag.getAttribute("type");

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                if (type == "bold") {
                    buf.append("<span class=\"bold\">");
                }
                else if (type == "illuminated") {
                    buf.append("<span class=\"illuminated\">");
                }
                else if (type == "italic") {
                    buf.append("<span class=\"italic\">");
                }
                else if (type == "line-through") {
                    buf.append("<span class=\"line-through\">");
                }
                else if (type == "normal") {
                    buf.append("<span class=\"normal\">");
                }
                else if (type == "small-caps") {
                    buf.append("<span class=\"small-caps\">");
                }
                else if (type == "underline") {
                    buf.append("<span class=\"underline\">");
                }
                else {
                    buf.append("<span>"); //don't break markup, </span> is inserted later
                }
            }
            else if (tag.isEndTag()) { //all hi replacements are html spans
                buf.append("</span>");
            }
        }

        //name
        else if (!strcmp(tag.getName(), "name")) {
            const sword::SWBuf type = tag.getAttribute("type");

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                if (type == "geographic") {
                    buf.append("<span class=\"name\"><span class=\"geographic\">");
                }
                else if (type == "holiday") {
                    buf.append("<span class=\"name\"><span class=\"holiday\">");
                }
                else if (type == "nonhuman") {
                    buf.append("<span class=\"name\"><span class=\"nonhuman\">");
                }
                else if (type == "person") {
                    buf.append("<span class=\"name\"><span class=\"person\">");
                }
                else if (type == "ritual") {
                    buf.append("<span class=\"name\"><span class=\"ritual\">");
                }
                else {
                    buf.append("<span class=\"name\"><span>");
                }
            }
            else if (tag.isEndTag()) { //all hi replacements are html spans
                buf.append("</span></span> ");
            }
        }
        else if (!strcmp(tag.getName(), "transChange")) {
            sword::SWBuf type( tag.getAttribute("type") );

            if ( !type.length() ) {
                type = tag.getAttribute("changeType");
            }

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                if (type == "added") {
                    buf.append("<span class=\"transchange\" title=\"");
                    buf.append(QObject::tr("Added text").toUtf8().constData());
                    buf.append("\"><span class=\"added\">");
                }
                else if (type == "amplified") {
                    buf.append("<span class=\"transchange\"><span class=\"amplified\">");
                }
                else if (type == "changed") {
                    buf.append("<span class=\"transchange\"><span class=\"changed\">");
                }
                else if (type == "deleted") {
                    buf.append("<span class=\"transchange\"><span class=\"deleted\">");
                }
                else if (type == "moved") {
                    buf.append("<span class=\"transchange\"><span class=\"moved\">");
                }
                else if (type == "tenseChange") {
                    buf.append("<span class=\"transchange\" title=\"");
                    buf.append(QObject::tr("Verb tense changed").toUtf8().constData());
                    buf.append("\"><span class=\"tenseChange\">");
                }
                else {
                    buf.append("<span class=\"transchange\"><span>");
                }
            }
            else if (tag.isEndTag()) { //all hi replacements are html spans
                buf.append("</span></span>");
            }
        }
        else if (!strcmp(tag.getName(), "p")) {
            if (tag.isEmpty()) {
                buf.append("<p/>");
            }
        }

        // <q> quote
        else if (!strcmp(tag.getName(), "q")) {
            //sword::SWBuf type = tag.getAttribute("type");
            sword::SWBuf who = tag.getAttribute("who");
            const char *lev = tag.getAttribute("level");
            int level = (lev) ? atoi(lev) : 1;
            sword::SWBuf quoteMarker = tag.getAttribute("marker");

            if ((!tag.isEndTag())) {
                if (!tag.isEmpty()) {
                	myUserData->quote.who = who;
                }

                if (quoteMarker.size() > 0) {
                    buf.append(quoteMarker);
                }
                else if (osisQToTick) //alternate " and '
                    buf.append((level % 2) ? '\"' : '\'');

                if (who == "Jesus") {
                    buf.append("<span class=\"jesuswords\">");
                }
            }
            else if (tag.isEndTag()) {
                if (myUserData->quote.who == "Jesus") {
                    buf.append("</span>");
                }
                if (quoteMarker.size() > 0) {
                    buf.append(quoteMarker);
                }
                else if (osisQToTick) { //alternate " and '
                    buf.append((level % 2) ? '\"' : '\'');
                }

                myUserData->quote.who = "";
            }
        }

        // abbr tag
        else if (!strcmp(tag.getName(), "abbr")) {
            if (!tag.isEndTag() && !tag.isEmpty()) {
                const sword::SWBuf expansion = tag.getAttribute("expansion");

                buf.append("<span class=\"abbreviation\" expansion=\"");
                buf.append(expansion);
                buf.append("\">");
            }
            else if (tag.isEndTag()) {
                buf.append("</span>");
            }
        }

        // <milestone> tag
        else if (!strcmp(tag.getName(), "milestone")) {
            const sword::SWBuf type = tag.getAttribute("type");

            if ((type == "screen") || (type == "line")) {//line break
                buf.append("<br/>");
                userData->supressAdjacentWhitespace = true;
            }
            else if (type == "x-p") { //e.g. occurs in the KJV2006 module
                //buf.append("<br/>");
                const sword::SWBuf marker = tag.getAttribute("marker");
                if (marker.length() > 0) {
                    buf.append(marker);
                }
            }
        }
        //seg tag
        else if (!strcmp(tag.getName(), "seg")) {
            if (!tag.isEndTag() && !tag.isEmpty()) {

                const sword::SWBuf type = tag.getAttribute("type");

                if (type == "morph") {//line break
                    //This code is for WLC and MORPH (WHI)
                    sword::XMLTag outTag("span");
                    outTag.setAttribute("class", "morphSegmentation");
                    const char* attrValue;
                    //Transfer the values to the span
                    //Problem: the data is in hebrew/aramaic, how to encode in HTML/BibleTime?
                    if ((attrValue = tag.getAttribute("lemma"))) outTag.setAttribute("lemma", attrValue);
                    if ((attrValue = tag.getAttribute("morph"))) outTag.setAttribute("morph", attrValue);
                    if ((attrValue = tag.getAttribute("homonym"))) outTag.setAttribute("homonym", attrValue);

                    buf.append(outTag.toString());
                    //buf.append("<span class=\"morphSegmentation\">");
                }
                else {
                    buf.append("<span>");
                }
            }
            else { // seg end tag
                buf.append("</span>");
            }
            //qWarning(QString("handled <seg> token. result: %1").arg(buf.c_str()).latin1());
        }

        //divine name, don't use simple tag replacing because it may have attributes
        else if (!strcmp(tag.getName(), "divineName")) {
            if (!tag.isEndTag()) {
                buf.append("<span class=\"name\"><span class=\"divine\">");
            }
            else { //all hi replacements are html spans
                buf.append("</span></span>");
            }
        }

        else { //all tokens handled by OSISHTMLHref will run through the filter now
            return sword::OSISHTMLHREF::handleToken(buf, token, userData);
        }
    }

    return false;
}

void Filters::BT_OSISHTML::renderReference(const char *osisRef, sword::SWBuf &buf, sword::SWModule *myModule, BT_UserData *myUserData) {
    QString ref( osisRef );
    QString hrefRef( ref );
    //Q_ASSERT(!ref.isEmpty()); checked later

    if (!ref.isEmpty()) {
        //find out the mod, using the current module makes sense if it's a bible or commentary because the refs link into a bible by default.
        //If the osisRef is something like "ModuleID:key comes here" then the
        // modulename is given, so we'll use that one

        CSwordModuleInfo* mod = CPointers::backend()->findSwordModuleByPointer(myModule);
        //Q_ASSERT(mod); checked later
        if (!mod || (mod->type() != CSwordModuleInfo::Bible
                     && mod->type() != CSwordModuleInfo::Commentary)) {

            mod = CBTConfig::get( CBTConfig::standardBible );
        }

        // Q_ASSERT(mod); There's no necessarily a module or standard Bible

        //if the osisRef like "GerLut:key" contains a module, use that
        int pos = ref.indexOf(":");

        if ((pos >= 0) && ref.at(pos - 1).isLetter() && ref.at(pos + 1).isLetter()) {
            QString newModuleName = ref.left(pos);
            hrefRef = ref.mid(pos + 1);

            if (CPointers::backend()->findModuleByName(newModuleName)) {
                mod = CPointers::backend()->findModuleByName(newModuleName);
            }
        }

        if (mod) {
            ReferenceManager::ParseOptions options;
            options.refBase = QString::fromUtf8(myUserData->key->getText());
            options.refDestinationModule = QString(mod->name());
            options.sourceLanguage = QString(myModule->Lang());
            options.destinationLanguage = QString("en");

            buf.append("<a href=\"");
            buf.append( //create the hyperlink with key and mod
                ReferenceManager::encodeHyperlink(
                    mod->name(),
                    ReferenceManager::parseVerseReference(hrefRef, options),
                    ReferenceManager::typeFromModule(mod->type())
                ).toUtf8().constData()
            );
            buf.append("\" crossrefs=\"");
            buf.append((const char*)ReferenceManager::parseVerseReference(ref, options).toUtf8().constData()); //ref must contain the osisRef module marker if there was any
            buf.append("\">");
        }
        // should we add something if there were no referenced module available?
    }
}

