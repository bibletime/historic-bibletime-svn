#!/bin/bash

### This defines the variables BT_LANG_ALL and BT_LANG_ONLINE
. ./languages.conf

### Defines the variable BIBLETIME_SVN_DIR
. ./local.conf

#param 1 == lang, param 2 == input dir
function createHtml {
	export polang=$1
	export in=$2

	export XML_CATALOG_FILES="docbook/docbook-xsl/catalog.xml docbook/docbook-xsl/website/catalog.xml docbook/catalog.xml"

	rm -f tmp/$polang/autolayout.xml
	xsltproc --nonet --output tmp/$polang/autolayout.xml docbook/custom-xsl/autolayout.xsl $in/layout.xml 
	xsltproc --nonet --path "$in" --stringparam output-root website-generated/$polang \
		--stringparam flags.languages "en $BT_LANG_ONLINE" \
		--stringparam l10n.gentext.default.language $polang  \
		docbook/custom-xsl/bibletime.xsl tmp/$polang/autolayout.xml
}

# param 1 == output filename for the .pot file
function updateTranslationTemplate {
	out=$1
	xml2po -o $out content/en/*.xml 
}

# This method merges i18n/lang.po with bt-svn/i18n/howto-lang.po if available and copiese to tmp/i18n/lang.po
# param 1 == language (e.g. de)
function mergePoTemplateIntoPoFile {	
	lang=$1

	echo "Merging i18n/en.pot into translation i18n/$lang.po"
	msgmerge --force-po -o tmp/temp-$lang.po "i18n/$lang.po" "i18n/en.pot"
	mv "tmp/temp-$lang.po" "i18n/$lang.po"

#	cp "i18n/$lang.po" "website-generated/po/bibletime_website_$lang.po"
}

# Creates the translated docbook files in tmp/lang/
# param 1 == language to generate the files for
function createTranslatedDocbook {
	POLANG=$1
	mkdir -p tmp/$POLANG

	for f in $(cd content/en/; ls *.xml); do \
		echo "Generating docbook: tmp/$POLANG/$f"; \
		xml2po --po-file="i18n/$POLANG.po" "content/en/$f" >  "tmp/$POLANG/$f"; \
	done;

	#make the Biblestudy howto .docbook files using the BibleTime docbook translation
	for f in $(cd content/en/; ls *.docbook); do \
		howto_po="$BIBLETIME_SVN_DIR/i18n/howto/howto-$POLANG.po"

		if test -f "$howto_po"; then \
			echo "Generating Bible study docbook: tmp/$POLANG/$f using $howto_po"; \
			cp $howto_po "tmp/biblestudy-$POLANG.po"
			xml2po --po-file="tmp/biblestudy-$POLANG.po" "content/en/$f" >  "tmp/$POLANG/$f"; \
			rm -f "tmp/biblestudy-$POLANG.po"
		else \
			echo "Using english Bile study file for tmp/$POLANG/$f"; \
			cp "content/en/$f" "tmp/$POLANG/$f"; \
		fi; \
	done;

}

# Fix xml files generated by xml2po, form with namespaces are broken, not needed for now.
function fixTranslation {
	POLANG=$1
	
	files="tmp/$POLANG/snippet_subscribe101.xml tmp/$POLANG/contact.xml tmp/$POLANG/snippet_subscribehandybirthdays.xml"
	for f in $files; do echo "Prostprocessing $f..."; cat $f | sed -e 's/<label/<html:label/' | 
		sed -e 's/<\/label/<\/html:label/' | 
		sed -e 's/<input/<html:input/' | 
		sed -e 's/<\/input/<\/html:input/' |
		sed -e 's/<option/<html:option/' | 
		sed -e 's/<\/option/<\/html:option/' |
		sed -e 's/<button/<html:button/' | 
		sed -e 's/<\/button/<\/html:button/' > $f; done;

	#Fix all files
	for f in tmp/$POLANG/product*.xml; do cat $f | sed -e 's/-de\.jpg/-en.jpg/' > $f; done;
}

#Copies the common website files into the website-generated dir
function copyResources {
	export _cpcmd="cp"
	if test $OSTYPE = FreeBSD; then export _cpcmd="gcp"; fi
	${_cpcmd} -Rafu website-common/* website-generated/
	find website-generated/ -iname \.svn | xargs rm -rf
}

###### THis is the main part of the application ########
mkdir -p tmp website-generated/postats website-generated/po;

#if test ! -h tmp/website-schema; then cd tmp; ln -s ../docbook/docbook-xsl/website/schema; fi
#if test ! -h content/website-schema; then cd content; ln -s ../docbook/docbook-xsl/website/schema; fi

createHtml en content/en


updateTranslationTemplate i18n/en.pot

for lang in $BT_LANG_ALL; do \
   mergePoTemplateIntoPoFile $lang; \
   createTranslatedDocbook $lang; \
   createHtml $lang "tmp/$lang"; \
done;

#Update the po statistics for all languages of the website
./scripts/update_postats.sh
./scripts/generate-sitemap.sh
./scripts/generate-apache-files.sh

copyResources 

#Cleanup
rm ./.xml2po.mo
rm -rf ./.tmp.i18n

echo -e "\nThe website is now available in website-generated ..."

#END
