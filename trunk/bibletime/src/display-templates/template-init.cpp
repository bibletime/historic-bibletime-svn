//THIS CODE IS AUTOGENERATED BY CREATE_CODE.PL. DO NOT EDIT!

void CDisplayTemplateMgr::init() {
	m_templateMap[ i18n("Default") ] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
<head>\n\
	<title>#TITLE#</title>\n\
	<meta name=\"GENERATOR\" content=\"BibleTime - www.bibletime.info\" />\n\
	<meta name=\"AUTHOR\" content=\"BibleTime - www.bibletime.info\" />\n\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n\
	\n\
	<style type=\"text/css\">\n\
	/* <![CDATA[ */\n\
\n\
#LANG_CSS#\n\
\n\
\n\
#content {\n\
	background-color:transparent;\n\
	margin:10px;\n\
}\n\
#content > table {\n\
	margin: 0;\n\
	padding: 0;\n\
	border-spacing:10px;\n\
}\n\
#content > table th {\n\
	padding: 0 0 2px 0;\n\
	text-align: center;\n\
	font-weight: bold;\n\
	font-size: 110%;\n\
	border-bottom: 1px dotted #7B7B7B;\n\
}\n\
\n\
a {\n\
	text-decoration:none;\n\
	font-weight:normal;\n\
	color: #7B7B7B;\n\
	padding:0 2px;\n\
	background-color:transparent;\n\
}\n\
a:hover {\n\
	text-decoration:none;\n\
	background-color:transparent;\n\
	color: #0000FF;\n\
	padding:0 2px;\n\
}\n\
\n\
\n\
/* Settings which apply to all entries in all modules */\n\
.entry {\n\
	padding: 2px;\n\
 	/*vertical-align: middle;*/\n\
	vertical-align: top;\n\
	text-align: justify;\n\
}\n\
td.entry {\n\
	vertical-align: top;\n\
 	text-align: justify !important;\n\
}\n\
#printer .entry {\n\
	font-size: 90%;\n\
	text-align: left;\n\
}\n\
.entry[dir=rtl] {\n\
	text-align: right !important;\n\
}\n\
#printer .entry[dir=rtl] {\n\
	text-align:right !important;\n\
}\n\
\n\
/* Keep the values the same as in .entry (take the border into account!) */\n\
.currententry {\n\
	padding: 2px;\n\
 	vertical-align: middle;\n\
	text-align: justify;\n\
	background-color: #E6E4FF;\n\
}\n\
.currententry[dir=rtl] {\n\
	text-align:right !important;\n\
}\n\
div.currententry {\n\
}\n\
span.currententry {\n\
	background-color: #EDEFFF !important;\n\
}\n\
\n\
\n\
/* Space between the columns of modules displayed side by side */\n\
td.entry + td.entry { \n\
	margin-left: 5px;\n\
}\n\
\n\
/* Several additional settings for display of modules*/\n\
#bible div.currententry, #bible td.currententry {\n\
	border: 1px solid #B7B7B7;\n\
	padding: 2px;\n\
}\n\
\n\
\n\
/*Same for books!*/\n\
#book div.currententry, #book td.currententry {\n\
	border: 1px solid #B7B7B7;\n\
	padding: 2px;\n\
}\n\
#book .entryname + .entry {\n\
	margin-top:2px;\n\
	padding-top:1px;\n\
}\n\
/* To remove the gap between the entrynam and the entry's content*/\n\
#book .entry > *, #book .currententry > * {\n\
	margin-top:0px;\n\
	padding-top:0px;\n\
}\n\
\n\
.entryname {\n\
	font-size:80%;\n\
	padding:0 2px 0 0;\n\
	vertical-align:super;\n\
}\n\
.entry[dir=rtl] .entryname, .currententry[dir=rtl] .entryname {\n\
	padding: 0 0 0 2px;\n\
	font-size: 80%;\n\
	vertical-align: super;\n\
}\n\
.entryname a, #printer .entryname  {\n\
}\n\
\n\
#printer .entry  .entry, #printer .entry .sectiontitle , #printer .entry .booktitle {\n\
	padding-top: 0.05em;\n\
	margin-left: 10px;\n\
}\n\
\n\
.footnote {\n\
	vertical-align:super;\n\
	font-weight: normal;\n\
	color: blue;\n\
	font-size: 80%;\n\
	cursor: help;\n\
	background-color:transparent;\n\
}\n\
.footnote:before { content:\"(\"; }\n\
.footnote:after { content:\")\"; }\n\
\n\
.footnotepre {\n\
 	font-style: italic;\n\
}\n\
\n\
\n\
.crossreference {\n\
	font-size: 80%;\n\
	color: blue;\n\
}\n\
#bible .crossreference:before  { content:\" [\"; }\n\
#bible .crossreference:after  { content:\"]\"; }\n\
\n\
.crossreference a {\n\
	color: blue;\n\
}\n\
.crossreference a:hover {\n\
	color: blue;\n\
}\n\
\n\
.alternative {}\n\
.alternative:before { content:\"'\"attr(alternative); }\n\
.alternative:after{ content:\"'\";}\n\
\n\
.abbreviation {\n\
\n\
}\n\
\n\
.sectiontitle {\n\
	font-weight: bold;\n\
	font-size: 110%;\n\
}\n\
#printer .sectiontitle {\n\
	font-weight: bold;\n\
	font-size: 100%;\n\
}\n\
* + .sectiontitle {\n\
	margin-top:15px;\n\
}\n\
\n\
.booktitle {\n\
	font-weight: bold;\n\
	font-size: 120%;\n\
	font-variant: small-caps;\n\
}\n\
#printer .booktitle {\n\
	font-weight: bold;\n\
	font-size: 120%;\n\
	font-variant: small-caps;\n\
}\n\
.foreign {\n\
}\n\
.jesuswords {\n\
	color:#9C2323;\n\
}\n\
\n\
.introduction {\n\
	font-size:80%;\n\
	text-align:justify;\n\
}\n\
\n\
.quotation {\n\
}\n\
.poetry {\n\
}\n\
.sup {\n\
	vertical-align: super;\n\
}\n\
.sub {\n\
	vertical-align: sub;\n\
}\n\
.right {\n\
	text-align: right;\n\
}\n\
.center {\n\
	text-align: center;\n\
}\n\
\n\
.bold {\n\
	font-weight:bold;\n\
}\n\
.illuminated {\n\
}\n\
.italic {\n\
	font-style:italic;\n\
}\n\
.line-through {\n\
}\n\
.normal {\n\
}\n\
.small-caps{\n\
}\n\
.underline {\n\
	font-style:underline;\n\
}\n\
\n\
.inscription {\n\
}\n\
.mentioned {\n\
}\n\
.name > .geographic {\n\
}\n\
.name > .holiday {\n\
}\n\
.name > .nonhuman {\n\
}\n\
.name > .person {\n\
}\n\
.name > .ritual {\n\
}\n\
.name > .divine {\n\
   font-variant: small-caps;\n\
}\n\
\n\
.transchange { \n\
	font-style:italic; \n\
}\n\
.transchange > .added {\n\
	background-color:inherit;\n\
}\n\
.transchange > .amplified {	\n\
}\n\
.transchange > .changed {	\n\
}\n\
.transchange > .deleted {\n\
}\n\
.transchange > .moved {\n\
}\n\
.transchange > .tenseChange {\n\
}\n\
\n\
.morphSegmentation {\n\
	border-right: 2px solid gray;\n\
}\n\
\n\
#infodisplay {\n\
	margin:0;\n\
	padding:0;\n\
}\n\
\n\
#infodisplay .footnoteinfo h3, #infodisplay .strongsinfo h3, #infodisplay .morphinfo h3, #infodisplay .translationinfo h3, #infodisplay .crossrefinfo h3 {\n\
	padding:4px 4px 2px 0;\n\
	color:darkGray;\n\
	font-weight:bold;\n\
	border-bottom:1px solid gray;\n\
	font-size:90%;\n\
	text-align: right;\n\
}\n\
\n\
#infodisplay > p, #infodisplay * > p,\n\
#infodisplay > .para, #infodisplay * > .para\n\
{ /* We only format the first p child in a special way */\n\
	font-size:95%;\n\
	text-align:justify !important;\n\
	color:black;\n\
	margin:0px;\n\
	padding:0px;\n\
}\n\
\n\
/*Required for cross references*/\n\
#infodisplay .entry {\n\
	padding:2px;\n\
	text-align:justify !important;\n\
	font-size:95%;\n\
	margin-bottom:10px;\n\
	margin-left:15px;\n\
}\n\
#infodisplay .entry[dir=rtl] {\n\
	text-align: right !important;\n\
}\n\
#infodisplay .entry .entry { /*No smaller font size for nested entries*/\n\
	font-size:100%;\n\
	margin:0px;\n\
}\n\
#infodisplay .entry .entry[dir=rtl] { /*No smaller font size for nested entries*/\n\
	text-align: right !important;\n\
}\n\
#infodisplay .entryname {\n\
	padding:2px;\n\
	margin:0px;\n\
	text-align:left !important;\n\
	font-weight:bold;	\n\
	font-size:100%;\n\
}\n\
\n\
#infodisplay .rangeheading {\n\
	color:black;\n\
	font-weight:bold;\n\
	margin-left:-10px;\n\
}\n\
\n\
#printer .rangeheading {\n\
	border-bottom:1px solid black;\n\
	color:black;\n\
}\n\
\n\
	/* ]]> */\n\
	</style>\n\
</head>\n\
\n\
<body id=\"#DISPLAYTYPE#\">\n\
	<div id=\"content\" lang=\"#LANG_ABBREV#\" xml:lang=\"#LANG_ABBREV#\" dir=\"#PAGE_DIRECTION#\">#CONTENT#</div>\n\
</body>\n\
</html>\n\
";
	m_templateMap[ i18n("Crazy") ] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
<head>\n\
	<!--TODO: This needs a better title than \"Crazy\"-->\n\
	<title>#TITLE#</title>\n\
	<meta name=\"GENERATOR\" content=\"BibleTime - www.bibletime.info\" />\n\
	<meta name=\"AUTHOR\" content=\"BibleTime - www.bibletime.info\" />\n\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n\
\n\
	<style type=\"text/css\">\n\
/* <![CDATA[ */\n\
\n\
body {\n\
	margin:0px;\n\
	padding:0;\n\
	padding:1em;\n\
	/*background-color:transparent;*/\n\
}\n\
\n\
\n\
\n\
/* content wraps all of the displayed elements */\n\
#content {\n\
	margin:0px;\n\
	padding:0px;\n\
	font-size:12pt;\n\
	text-align:justify;\n\
}\n\
\n\
/* The table contains the Bible verses, each verse on an own row, each Bible module in an own column */\n\
#content > table {\n\
	margin:0;\n\
	padding:3px;\n\
	border-spacing:0;\n\
	vertical-align:top;\n\
	text-align:justify;\n\
}\n\
/* The heading contains the name of the module and may contain additional information like the display keys */\n\
#content > table th {\n\
	text-shadow:black 1px 1px 3px;\n\
	font-size:1.3em;\n\
	background-color:#F0F0F0;\n\
	padding:8px;\n\
	border-bottom:1px solid black;\n\
}\n\
\n\
#content tr:nth-child(odd) { background-color: white; }\n\
#content div.entry:nth-child(odd) { background-color: white; }\n\
\n\
#content tr:nth-child(even)  { background-color: #EDEDED; }\n\
#content div.entry:nth-child(even)  { background-color: #EDEDED; }\n\
/*#content tr:nth-child(even)  { background-color: #D0D0D0; }\n\
#content div.entry:nth-child(even)  { background-color: #D0D0D0; }*/\n\
\n\
\n\
/* The a links are the normal HTML hyperlinks. Links without a class attribute are hyperlinks into the web */\n\
a { color:#FF0004; padding:0; }\n\
a:hover { }\n\
\n\
/* An entry is the generic element used for all content items.\n\
 In Bibles an entry contains a verse, in commentaries the comment to a verse range,\n\
 in lexicons an simple item and in book the text of an item with other .entry elements as childs.\n\
\n\
 If just one module is chosen, all entries are put together in one sourrounding table column. In this case the elements div and span may have the class \"entry\". If it's a div each entry should be on it's own line. span.entry should not add a linebreak.\n\
\n\
 If more than one module (Bibles) was chosen, the entry class applies to \"td\" elements, one column for a verse\n\
 */\n\
.entry {\n\
	padding:0.3em;\n\
	vertical-align: top;\n\
}\n\
table .entry:nth-child(odd) {\n\
	border-left:10px solid #DEE6F6;\n\
}\n\
\n\
table .entry:nth-child(even) {\n\
	border-left:10px solid #E6EEFF;\n\
}\n\
\n\
#printer .entry { text-indent:15px; }\n\
/* Often needed to format BiDi text in the right way */\n\
/* .entry[dir=rtl] { text-align:right !important; } */b\n\
/* #printer .entry[dir=rtl] { } */\n\
\n\
/* Special formatting for the block element entries */\n\
div.entry, td.entry { }\n\
td.entry {\n\
	vertical-align:top;\n\
}\n\
\n\
/* Currententry is chosen when the entry should be highlighted as the current entry chosen by the user\n\
 currententry may apply to span, div, td (just like .entry)\n\
 */\n\
.currententry {\n\
	padding:0.3em;\n\
	line-height:1.6em;\n\
	background-color:#D3E5FF;\n\
	/*border: thin solid black;*/\n\
	\n\
}\n\
\n\
table .currententry:nth-child(1) {\n\
	margin-left:0;\n\
	border-left:10px solid #99B4FF;\n\
	/*border-left:10px solid #F5FF6A;*/\n\
}\n\
/* .currententry[dir=rtl] { */\n\
/* } */\n\
div.currententry { }\n\
span.currententry { }\n\
\n\
/* You may use this to format the space between two columns */\n\
td.entry + td.entry {\n\
	margin:0;\n\
}\n\
\n\
/* Several additional settings for displaying Bible modules*/\n\
#bible div.entry {\n\
	border-left:10px solid white;\n\
}\n\
#bible div.entry[dir=rtl] {\n\
	border-left:none;\n\
	border-right:10px solid white;\n\
	text-align:right;\n\
}\n\
#bible div.currententry {\n\
	border-left:10px solid #99B4FF;\n\
}\n\
#bible div.currententry[dir=rtl] {\n\
	border-left:none;\n\
	border-right:10px solid #99B4FF;\n\
}\n\
#bible div.currententry, #bible td.currententry { }\n\
#bible td.currententry { }\n\
\n\
/* Same for books! */\n\
#book div.currententry, #book td.currententry { }\n\
#book .entryname + .entry { }\n\
/* To remove the gap between the entryname and the entry's content*/\n\
#book .entry > *, #book .currententry > * { }\n\
\n\
/* An entryname is embedded in an .entry tag and contains the name of the key, which belongs to the text of the .entry */\n\
.entryname { size:60%; vertical-align:super;}\n\
.entryname a { text-decoration:none !important; color:#8F2627 !important; font-style:italic; padding:0; }\n\
\n\
.entry[dir=rtl] .entryname, .currententry[dir=rtl] .entryname { }\n\
.entryname a, #printer .entryname { }\n\
/*  As you can see #printer is used to format pages which are send to the printer. Printer pages should be clean and easy to read */\n\
#printer .entry  .entry, #printer .entry .sectiontitle , #printer .entry .booktitle { }\n\
\n\
/* Footnote formatting options */\n\
/* A footnote is displayed as an asterisk or another small character. You can set the colors, font-size, etc. for the footnote. */\n\
.footnote { color:#8F2627; vertical-align:super; font-size:70%;}\n\
/* You may use this to add sourrounding brackets in Bible modules. To do this the content command of CSS */\n\
#bible .footnote:before { content:\" [\"; color:#8F2627; }\n\
#bible .footnote:after { content:\"]\"; color:#8F2627; }\n\
\n\
/* Crossreferences. The same as footnotes */\n\
.crossreference { color:#8f2627;}\n\
#bible .crossreference:before { content:\" {\" }\n\
#bible .crossreference:after { content:\"}\" }\n\
\n\
/*Keep in mind that for cross references it is not enough to\n\
*set only the cross reference colour, you must also set the\n\
*cross reference anchor marker with .crossreference a\n\
*This way both the non linked and linked text will show\n\
*as desired. Additionally, you can have seperate formatting\n\
*for linked and non linked text in the cross reference*/\n\
.crossreference a {\n\
   color:#8f2627;\n\
   /*Get rid of underlines on the notes when not hovered*/\n\
   text-decoration: none;\n\
}\n\
.crossreference a:hover {}\n\
\n\
/* Alternative reading as defined in the OSIS specs. The information about the alternative text is displayed in the info display. It's\n\
often a good idea to format this the same as footnotes\n\
The text which is the alternative of the displayed text is available in the attribute \"alternative\"\n\
so something like .alternative:before { content:\"'\"attr(alternative); } is possible\n\
*/\n\
.alternative {}\n\
.alternative:before { }\n\
.alternative:after { }\n\
\n\
/* An abbreviation. */\n\
.abbreviation { }\n\
\n\
/* A title within the text. This is the formatting which applies to a title of a section, as you can find them in Bibles */\n\
.sectiontitle {\n\
	 padding:0.3em;\n\
	 /*text-shadow:gray 0px 0px 3px;*/\n\
	 font-size:1.3em;\n\
	 font-weight:bold;\n\
	 text-transform:uppercase;\n\
	 padding-bottom:2px;\n\
}\n\
#printer .sectiontitle { }\n\
/* You may want to format elements before section titles in a special way. */\n\
* + .sectiontitle { }\n\
\n\
/* The title of a book, e.g. \"The gospel of Matthew\" */\n\
.booktitle { }\n\
#printer .booktitle { }\n\
\n\
/* A foreign word */\n\
.foreign { }\n\
\n\
/* Words spoken by Jesus Christ */\n\
/*.jesuswords { color: red; }*/\n\
.jesuswords { color: #B60000; }\n\
\n\
/* An introduction to a module, book or chapter */\n\
.introduction { }\n\
/* A quotation without a special speaker */\n\
.quotation { }\n\
/* Poetry formatting */\n\
.poetry { }\n\
/* Text displayed above. It's like the sup tag in HTML */\n\
.sup { }\n\
/* Sub text, the opposite of .sup */\n\
.sub { }\n\
/* Display the text inside as right aligned text */\n\
.right { }\n\
/* Display as centered text */\n\
.center { }\n\
\n\
/* Visual attributes to the text */\n\
.bold {\n\
   font-weight: bold;\n\
}\n\
.illuminated { }\n\
.italic {\n\
   font-style: italic;\n\
}\n\
.line-through { }\n\
.normal { }\n\
.small-caps{ }\n\
.underline { }\n\
\n\
/* An inscription */\n\
.inscription { }\n\
\n\
.mentioned { }\n\
\n\
/* Special formatting rules for names, see the OSIS specs for detailed information */\n\
.name {}\n\
.name > .geographic { }\n\
.name > .holiday { }\n\
.name > .nonhuman { }\n\
.name > .person { }\n\
.name > .ritual { }\n\
.name > .divine { font-variant: small-caps; }\n\
\n\
/* Information added by the translator */\n\
.transchange {}\n\
.transchange > .added { }\n\
.transchange > .amplified {}\n\
.transchange > .changed { }\n\
.transchange > .deleted { }\n\
.transchange > .moved { }\n\
.transchange > .tenseChange { }\n\
\n\
/* Special text markup */\n\
/* Morph segmentation is a special feature for hebrew to markup the word divisions in the text */\n\
/* .morphSegmentation { border-left:1px solid red; } */\n\
.morphSegmentation + .morphSegmentation {\n\
	border-right: 1px red solid;\n\
}\n\
\n\
/* The formatting of all things displayed in the information window */\n\
#infodisplay {background:white; font-size:10pt; padding:5px; margin:0px;}\n\
\n\
/* The headings of a section, several sections may be displayed together at the same time */\n\
#infodisplay .footnoteinfo h3,\n\
#infodisplay .strongsinfo h3,\n\
#infodisplay .morphinfo h3,\n\
#infodisplay .translationinfo h3,\n\
#infodisplay .crossrefinfo h3 {\n\
	font-size:12pt;\n\
	font-weight:bold;\n\
	text-transform:uppercase;\n\
	text-align:right;\n\
	color:gray;\n\
	border-bottom:1px solid black;\n\
	padding:4px;\n\
}\n\
\n\
#infodisplay > p, #infodisplay * > p { /* We only format the first p child in a special way */\n\
	font-size:80%;\n\
}\n\
\n\
/*Required for cross references*/\n\
#infodisplay .entry {\n\
	font-size:80%;\n\
}\n\
#infodisplay .entry .entry { /* No smaller font size for nested entries */\n\
	font-size:100% !important;\n\
}\n\
#infodisplay .entryname {\n\
	font-weight:bold;\n\
}\n\
#infodisplay .rangeheading {\n\
	font-weight:bold;\n\
}\n\
\n\
#printer .rangeheading {}\n\
\n\
\n\
*[dir=rtl] { text-align:right !important; }\n\
\n\
/* Here's the marker for the language specific fotn information. BibleTime replaces this by the CSS for the fonts */\n\
#LANG_CSS#\n\
\n\
/* ]]> */\n\
	</style>\n\
</head>\n\
\n\
<!--  The display type may be bible, book, printer -->\n\
<body id=\"#DISPLAYTYPE#\">\n\
	<div id=\"content\" lang=\"#LANG_ABBREV#\" xml:lang=\"#LANG_ABBREV#\" dir=\"#PAGE_DIRECTION#\">#CONTENT#</div>\n\
</body>\n\
</html>\n\
";
	m_templateMap[ i18n("Green") ] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
<head>\n\
	<title>#TITLE#</title>\n\
	<meta name=\"GENERATOR\" content=\"BibleTime - www.bibletime.info\" />\n\
	<meta name=\"AUTHOR\" content=\"BibleTime - www.bibletime.info\" />\n\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n\
	\n\
	<style type=\"text/css\">\n\
	/* <![CDATA[ */\n\
\n\
\n\
\n\
#content {\n\
	background-color:transparent;\n\
	margin:10px;\n\
}\n\
#content > table {\n\
	margin: 0;\n\
	padding: 0;\n\
	border-spacing:10px;\n\
	vertical-align:top;\n\
}\n\
#content > table th {\n\
	padding: 0 0 2px 0;\n\
	text-align: center; \n\
	font-weight: bold;\n\
	font-size: 110%;\n\
	border-bottom: 1px solid #6B6B6B;\n\
}\n\
\n\
a {\n\
	text-decoration:none;\n\
	font-weight:normal;\n\
	color: #3B11AE;\n\
	background-color:transparent;	\n\
	padding:0 2px;\n\
}\n\
a:hover {\n\
	background-color:transparent;\n\
	color: #AE1518;\n\
	padding:0 2px;\n\
}\n\
\n\
\n\
/* Settings which apply to all entries in all modules */\n\
.entry {\n\
	padding: 3px;\n\
/* 	vertical-align: middle; */\n\
	vertical-algin: top;\n\
	text-align: justify;\n\
}\n\
#printer .entry {\n\
	font-size:90%;\n\
	text-align:left;\n\
}\n\
.entry[dir=rtl] {\n\
	text-align:right !important;\n\
}\n\
#printer .entry[dir=rtl] {\n\
	text-align:right !important;\n\
}\n\
div.entry, td.entry {\n\
	vertical-align:top;\n\
}\n\
\n\
/* Keep the values the same as in .entry (take the border into account!) */\n\
.currententry {\n\
	padding:3px;\n\
	vertical-align:middle;\n\
	text-align:justify;\n\
}\n\
.currententry[dir=rtl] {\n\
	text-align:right !important;\n\
}\n\
div.currententry {\n\
}\n\
span.currententry {\n\
	background-color:#D4FFCB;\n\
}\n\
\n\
\n\
/*Space between the columns of modules displayed side by side*/\n\
td.entry + td.entry { \n\
	margin-left: 5px;\n\
}\n\
\n\
/* Several additional settings for display of modules*/\n\
#bible div.currententry, #bible td.currententry {\n\
	border:1px solid black;\n\
	padding:2px;\n\
	background-color:#F6FFF7;\n\
}\n\
#bible span.currententry {\n\
	background-color:#C8FFB6;\n\
}\n\
\n\
/*Same for books!*/\n\
#book div.currententry, #book td.currententry {\n\
	border:1px solid black;\n\
	background-color:#F6FFF7;\n\
}\n\
#book span.currententry {\n\
	background-color:#C8FFB6;\n\
}\n\
\n\
\n\
.entryname {\n\
	font-size:80%;\n\
	padding:0 2px 0 0;\n\
	vertical-align:super;\n\
}\n\
.entry[dir=rtl] .entryname, .currententry[dir=rtl] .entryname {\n\
	padding: 0 0 0 2px;\n\
	font-size: 80%;\n\
	vertical-align:super;\n\
}\n\
.entryname a, #printer .entryname  {\n\
	vertical-align: middle\n\
}\n\
\n\
#printer .entry  .entry, #printer .entry .sectiontitle , #printer .entry .booktitle {\n\
	padding-top: 0.05em;\n\
	margin-left: 10px;\n\
}\n\
\n\
.footnote {\n\
	vertical-align:super;\n\
	color: blue;\n\
	font-size: 70%;\n\
}\n\
.footnote:before { content:\"(\"; }\n\
.footnote:after { content:\")\"; }\n\
\n\
.crossreference {\n\
	font-size: 80%;\n\
	/*vertical-align:middle;*/\n\
	color: blue;\n\
}\n\
\n\
.crossreference a {\n\
	font-size: 80%;\n\
	/*vertical-align:middle;*/\n\
	color: blue;\n\
}\n\
\n\
.crossreference a:hover {\n\
	font-size: 80%;\n\
	/*vertical-align:middle;*/\n\
	color: blue;\n\
}\n\
\n\
\n\
\n\
.sectiontitle {\n\
	font-weight: bold;\n\
	font-size: 110%;\n\
}\n\
#printer .sectiontitle {\n\
	font-weight: bold;\n\
	font-size: 100%;\n\
}\n\
* + .sectiontitle {\n\
	margin-top:15px;\n\
}\n\
\n\
.booktitle {\n\
	font-weight: bold;\n\
	font-size: 120%;\n\
	font-variant: small-caps;\n\
}\n\
#printer .booktitle {\n\
	font-weight: bold;\n\
	font-size: 120%;\n\
	font-variant: small-caps;\n\
}\n\
.foreign {\n\
}\n\
.jesuswords {\n\
	color:#9C2323;\n\
}\n\
\n\
.quotation {\n\
}\n\
.poetry {\n\
}\n\
.sup {\n\
	vertical-align: super;\n\
}\n\
.sub {\n\
	vertical-align: sub;\n\
}\n\
.right {\n\
	text-align: right;\n\
}\n\
.center {\n\
	text-align: center;\n\
}\n\
\n\
.bold {\n\
	font-weight:bold;\n\
}\n\
.illuminated {\n\
}\n\
.italic {\n\
	font-style:italic;\n\
}\n\
.line-through {\n\
}\n\
.normal {\n\
}\n\
.small-caps{\n\
}\n\
.underline {\n\
	font-style:underline;\n\
}\n\
\n\
.inscription {\n\
}\n\
.mentioned {\n\
}\n\
.name > .geographic {\n\
}\n\
.name > .holiday {\n\
}\n\
.name > .nonhuman {\n\
}\n\
.name > .person {\n\
}\n\
.name > .ritual {\n\
}\n\
.name > .divine {\n\
   font-variant: small-caps;\n\
}\n\
\n\
.transchange { \n\
	font-style:italic; \n\
}\n\
.transchange > .added {\n\
	background-color:inherit;\n\
}\n\
.transchange > .amplified {	\n\
}\n\
.transchange > .changed {	\n\
}\n\
.transchange > .deleted {\n\
}\n\
.transchange > .moved {\n\
}\n\
.transchange > .tenseChange {\n\
}\n\
\n\
.morphSegmentation {\n\
	border: 1px solid gray;\n\
	margin-top: 1px; /* Don`t let the boxes touch here, between the lines */\n\
	margin-left: -1px; /* But here, for better reading of the text, \n\
											this will only occur within a word consisting of several segments.*/\n\
	margin-right: 0px;\n\
}\n\
\n\
#infodisplay {\n\
	margin:0;\n\
	padding:0;\n\
	font-size:100%;\n\
}\n\
\n\
#infodisplay .footnoteinfo h3, #infodisplay .strongsinfo h3, #infodisplay .morphinfo h3, #infodisplay .translationinfo h3, #infodisplay .crossrefinfo h3 {\n\
	padding:4px 4px 2px 0;\n\
	color:darkGray;\n\
	font-weight:bold;\n\
	border-bottom:1px solid gray;\n\
	font-size:100%;\n\
	text-align:left !important;\n\
}\n\
\n\
#infodisplay > p, #infodisplay  * > p { /* We only format the first p child in a special way */\n\
	font-size:100%;\n\
	text-align:justify;\n\
	color:black;\n\
	margin:0px;\n\
	padding:0px;\n\
}\n\
\n\
/*Required for cross references*/\n\
#infodisplay .entry {\n\
	padding:2px;\n\
	text-align:justify;\n\
	font-size:100%;\n\
	margin-bottom:10px;\n\
	margin-left:15px;\n\
}\n\
#infodisplay .entry .entry { /*No smaller font size for nested entries*/\n\
	font-size:100%;\n\
	margin:0px;\n\
}\n\
#infodisplay .entryname {\n\
	padding:2px;\n\
	margin:0px;\n\
	text-align:left;\n\
	font-weight:bold;	\n\
	font-size:100%;\n\
}\n\
\n\
#infodisplay .rangeheading {\n\
	color:black;\n\
	font-weight:bold;\n\
	margin-left:-10px;\n\
}\n\
\n\
#printer .rangeheading {\n\
	border-bottom:1px solid black;\n\
	color:black;\n\
}\n\
\n\
#LANG_CSS#\n\
\n\
	/* ]]> */\n\
	</style>\n\
</head>\n\
\n\
<body id=\"#DISPLAYTYPE#\">\n\
	<div id=\"content\" lang=\"#LANG_ABBREV#\" xml:lang=\"#LANG_ABBREV#\" dir=\"#PAGE_DIRECTION#\">#CONTENT#</div>\n\
</body>\n\
</html>\n\
";
	m_templateMap[ i18n("High contrast") ] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
<head>\n\
	<title>#TITLE#</title>\n\
	<meta name=\"GENERATOR\" content=\"BibleTime - www.bibletime.info\" />\n\
	<meta name=\"AUTHOR\" content=\"BibleTime - www.bibletime.info\" />\n\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n\
	\n\
	<style type=\"text/css\">\n\
	/* <![CDATA[ */\n\
\n\
\n\
#content {\n\
	/*background-color:white;*/\n\
	margin:10px;\n\
}\n\
#content > table {\n\
	margin: 0;\n\
	padding: 0;\n\
	border-spacing:10px;\n\
	vertical-align:top;\n\
}\n\
#content > table th {\n\
	padding: 0 0 2px 0;\n\
	text-align: center; \n\
	/*font-weight: bold;*/\n\
	font-size: 110%;\n\
	border-bottom: 1px solid #6B6B6B;\n\
}\n\
\n\
a {\n\
	text-decoration:none;\n\
	font-weight:normal;\n\
	/*color: #F98100;*/\n\
	padding:1px;\n\
	background-color:transparent;\n\
}\n\
a:hover {\n\
	background-color:transparent;\n\
	color: #FF0000;\n\
	padding:1px;\n\
	text-decoration:underline;\n\
}\n\
\n\
/* Settings which apply to all entries in all modules */\n\
.entry {\n\
	padding: 2px;\n\
	vertical-align: top;\n\
	text-align: justify;\n\
}\n\
#printer .entry {\n\
	font-size: 90%;\n\
	text-align: left;\n\
}\n\
.entry[dir=rtl],#printer .entry[dir=rtl]  {\n\
	text-align: right;\n\
}\n\
div.entry, td.entry {\n\
	vertical-align:top;	\n\
}\n\
\n\
/* Keep the values the same as in .entry (take the border into account!) */\n\
.currententry {\n\
	padding: 2px;\n\
	vertical-align: top;\n\
	text-align: justify;\n\
	/*font-weight: bolder;*/\n\
	/*background-color:transparent;*/\n\
	/*color:white;*/\n\
	/*background-color:black;*/\n\
	/*The background colour of selected text as of kde 3.5.2*/\n\
	background-color: #A5A5FF;\n\
}\n\
.currententry[dir=rtl] {\n\
	text-align: right;\n\
}\n\
div.currententry {\n\
}\n\
span.currententry {\n\
	\n\
}\n\
\n\
\n\
/*Space between the columns of modules displayed side by side*/\n\
td.entry + td.entry { \n\
	margin-left: 5px;\n\
}\n\
\n\
/* Several additional settings for display of modules*/\n\
#bible div.currententry, #bible td.currententry {\n\
	/*border: 1px solid black;*/\n\
	padding: 4px;\n\
}\n\
#bible span.currententry {\n\
}\n\
\n\
/*Same for books!*/\n\
#book div.currententry, #book td.currententry {\n\
	border: 1px solid black;\n\
}\n\
#book span.currententry {\n\
}\n\
\n\
\n\
.entryname {\n\
 	font-size:70%;\n\
	padding:0 1px 0 0;\n\
	vertical-align:top;\n\
	/*color: #F98100;*/\n\
	color: black;\n\
	/*font-style: italic;*/\n\
}\n\
.entry[dir=rtl] .entryname, .currententry[dir=rtl] .entryname {\n\
	padding:0 0 0 1px;\n\
	font-size:60%;\n\
	/*vertical-align:top; */\n\
}\n\
.entryname a, #printer .entryname  {\n\
	vertical-align: top;\n\
	font-size: 100%;\n\
	font-weight: bold;\n\
	color: black;\n\
}\n\
\n\
#printer .entry  .entry, #printer .entry .sectiontitle , #printer .entry .booktitle {\n\
	padding-top:0.05em;\n\
	margin-left:10px;\n\
}\n\
\n\
.footnote {\n\
	vertical-align:super;\n\
	font-weight:bold;\n\
	/*color: #00B7FF;*/ /*This one is kind of blinding on white*/\n\
	/*color: #F98100;*/\n\
	font-size:80%;\n\
	color: black;\n\
	content: \"1\";\n\
}\n\
\n\
\n\
.footnote:before {\n\
	content: \"(\";\n\
}\n\
\n\
.footnote:after {\n\
	content: \")\";\n\
}\n\
\n\
.crossreference {\n\
	font-size: 80%;\n\
	/*vertical-align:middle;*/\n\
	/*color: blue;*/\n\
	/*color: #F98100;*/\n\
}\n\
\n\
.crossreference:before {\n\
	content: \"[\";\n\
}\n\
\n\
.crosreference:after {\n\
	content: \"]\";\n\
}\n\
\n\
.sectiontitle {\n\
	font-weight: bold;\n\
	font-size: 110%;\n\
}\n\
#printer .sectiontitle {\n\
	font-weight: bold;\n\
	font-size: 100%;\n\
}\n\
* + .sectiontitle {\n\
	margin-top:15px;\n\
}\n\
\n\
.booktitle {\n\
	font-weight: bold;\n\
	font-size: 120%;\n\
	font-variant: small-caps;\n\
}\n\
#printer .booktitle {\n\
	font-weight: bold;\n\
	font-size: 120%;\n\
	font-variant: small-caps;\n\
}\n\
.foreign {\n\
}\n\
.jesuswords {\n\
	/*color:red;*/\n\
	/*color: #7500AC;*/\n\
	/*color: #F98100;*/\n\
	font-style:italic;\n\
	/*font-weight: bold;*/\n\
	font-size:0.9em;\n\
}\n\
\n\
.quotation {\n\
}\n\
.poetry {\n\
}\n\
.sup {\n\
	vertical-align: super;\n\
}\n\
.sub {\n\
	vertical-align: sub;\n\
}\n\
.right {\n\
	text-align: right;\n\
}\n\
.center {\n\
	text-align: center;\n\
}\n\
\n\
.bold {\n\
	font-weight:bold;\n\
}\n\
.illuminated {\n\
}\n\
.italic {\n\
	font-style:italic;\n\
}\n\
.line-through {\n\
}\n\
.normal {\n\
}\n\
.small-caps{\n\
}\n\
.underline {\n\
	font-style:underline;\n\
}\n\
\n\
.inscription {\n\
}\n\
.mentioned {\n\
}\n\
.name > .geographic {\n\
}\n\
.name > .holiday {\n\
}\n\
.name > .nonhuman {\n\
}\n\
.name > .person {\n\
}\n\
.name > .ritual {\n\
}\n\
.name > .divine {\n\
   font-variant: small-caps;\n\
}\n\
\n\
.transchange { \n\
	font-style:italic; \n\
}\n\
.transchange > .added {\n\
	background-color:inherit;\n\
}\n\
.transchange > .amplified {	\n\
}\n\
.transchange > .changed {	\n\
}\n\
.transchange > .deleted {\n\
}\n\
.transchange > .moved {\n\
}\n\
.transchange > .tenseChange {\n\
}\n\
\n\
.morphSegmentation {\n\
	border: 1px solid gray;\n\
	margin-top: 1px; /* Don`t let the boxes touch here, between the lines */\n\
	margin-left: -1px; /* But here, for better reading of the text, \n\
											this will only occur within a word consisting of several segments.*/\n\
	margin-right: 0px;\n\
}\n\
\n\
#infodisplay {\n\
	margin:0;\n\
	padding:0;\n\
}\n\
\n\
#infodisplay .footnoteinfo h3, #infodisplay .strongsinfo h3, #infodisplay .morphinfo h3, #infodisplay .translationinfo h3, #infodisplay .crossrefinfo h3 {\n\
	padding: 2px 0 0 0;\n\
	color: black;\n\
	font-weight: bold;\n\
	border-bottom: 1px solid black;\n\
	font-size: 90%;\n\
	margin: 0 0 3px 0;\n\
}\n\
\n\
#infodisplay > p { /* We only format the first p child in a special way */\n\
	font-size: 90%;\n\
	color: black;\n\
	margin: 0px;\n\
	padding: 0px;\n\
}\n\
\n\
/*Required for cross references*/\n\
#infodisplay .entry {\n\
	padding: 2px;\n\
	text-align: justify;\n\
	font-size: 80%;\n\
	margin: 0 0 10px 10px;\n\
}\n\
#infodisplay .entry .entry { /*No smaller font size for nested entries*/\n\
	font-size: 100%;\n\
	margin: 0px;\n\
}\n\
#infodisplay .entryname {\n\
	padding: 2px;\n\
	margin: 0px;\n\
	text-align: left;\n\
	font-weight: bold;\n\
	font-size: 90%;\n\
}\n\
\n\
#infodisplay .rangeheading {\n\
	color: black;\n\
	font-weight: bold;\n\
	margin-left: -5px;\n\
}\n\
\n\
#printer .rangeheading {\n\
	border-bottom: 1px solid black;\n\
	color: black;\n\
}\n\
\n\
#LANG_CSS#\n\
\n\
	/* ]]> */\n\
	</style>\n\
</head>\n\
\n\
<body id=\"#DISPLAYTYPE#\">\n\
	<div id=\"content\" lang=\"#LANG_ABBREV#\" xml:lang=\"#LANG_ABBREV#\">#CONTENT#</div>\n\
</body>\n\
</html>\n\
";
	m_templateMap[ i18n("Simple") ] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
<head>\n\
	<title>#TITLE#</title>\n\
	<meta name=\"GENERATOR\" content=\"BibleTime \" VERSION \"\" />\n\
	<meta name=\"AUTHOR\" content=\"BibleTime \" VERSION \"\" />\n\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n\
	\n\
	<style type=\"text/css\">\n\
/* <![CDATA[ */\n\
body {\n\
	margin:5px;\n\
}\n\
#content {\n\
	background-color:white;\n\
}\n\
#content table {\n\
	margin:0;\n\
	padding:0;\n\
	border-spacing:10px;\n\
	border-collapse: collapse;\n\
	vertical-align: top;\n\
}\n\
#content table th {\n\
	padding: 0 0 2px 0;\n\
	text-align: center; \n\
	font-weight: bold;\n\
	font-size: 115%;\n\
}\n\
\n\
a {\n\
	text-decoration:none;\n\
	font-weight:normal;\n\
	color: blue;\n\
	padding:2px;\n\
}\n\
\n\
td.entry, div.entry {\n\
	padding: 5px;\n\
	vertical-align: top;\n\
}\n\
span.entry {\n\
	padding: 5px;\n\
}\n\
\n\
td.currententry, div.currententry {\n\
	padding: 5px;\n\
	vertical-align: top;\n\
	font-weight:bold;\n\
}\n\
td.entry + td.entry {\n\
	margin-left: 5px;\n\
}\n\
span.currententry {\n\
	font-weight:bold;\n\
	vertical-align: middle;\n\
	padding: 5px;\n\
}\n\
\n\
.footnote {\n\
	color: gray;\n\
}\n\
\n\
.strongnumber {\n\
	vertical-align: top;\n\
	font-size: 60%;\n\
	color: blue;\n\
}\n\
.morphcode {\n\
	vertical-align: top;\n\
	font-size: 60%;\n\
	color: blue;\n\
}\n\
.lemma {\n\
}\n\
\n\
.sectiontitle {\n\
	font-weight: bold;\n\
	font-size: 120%;\n\
}\n\
.entry + .sectiontitle {\n\
	margin-top:35px;\n\
	padding-top:35px;\n\
}\n\
\n\
.booktitle {\n\
	font-weight: bold;\n\
	font-size: 140%;\n\
}\n\
.foreign {\n\
}\n\
.jesuswords {\n\
	color: red;\n\
	font-size: 0.9em;\n\
}\n\
\n\
.name > .divine {\n\
   font-variant: small-caps;\n\
}\n\
\n\
.quotation {\n\
}\n\
.poetry {\n\
}\n\
.sup {\n\
	vertical-align: super;\n\
}\n\
.sub {\n\
	vertical-align: sub;\n\
}\n\
.right {\n\
	text-align: right;\n\
}\n\
.center {\n\
	text-align: center;\n\
}\n\
/*TODO: Think of something better here since this will get confusing*/\n\
/*	on verses but, is needed for things inthe Mag/Info window*/\n\
.bold {\n\
	font-weight: bold;\n\
}\n\
.italic {\n\
	font-style: italic;\n\
}\n\
#LANG_CSS#\n\
/* ]]> */\n\
	</style>\n\
</head>\n\
\n\
<body>\n\
	<div id=\"content\" lang=\"#LANG_ABBREV#\" xml:lang=\"#LANG_ABBREV#\" dir=\"#PAGE_DIRECTION#\">#CONTENT#</div>\n\
</body>\n\
</html>\n\
";

}
