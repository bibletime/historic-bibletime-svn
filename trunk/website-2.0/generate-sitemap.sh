#!/bin/sh
. ./languages.conf

export XML_CATALOG_FILES="docbook/docbook-xsl/catalog.xml docbook/docbook-xsl/website/catalog.xml docbook/catalog.xml"

#Generate the languages files
echo "<languages>" > tmp/languages.xml;
for lang in en $BT_LANG_ALL; do \
  if test -f  "tmp/$lang/autolayout.xml"; then \
	echo "<language name=\"$lang\" file=\"$lang/autolayout.xml\"/>" >> tmp/languages.xml; \
  fi; \
done;
echo "</languages>" >> tmp/languages.xml;
	
xsltproc --nonet  \
	--stringparam change.date $(date +%Y-%m-%d) \
	--output website-generated/sitemap.xml \
	docbook/custom-xsl/sitemap.xsl tmp/languages.xml 

