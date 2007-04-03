#!/bin/bash
# This script recreates the translated pages without po file processing. e.g. if a xsl has changed.

for l in $(cat config/language-online.conf); do

export POLANG=$l
export XML_CATALOG_FILES="docbook-xsl/catalog.xml docbook-xsl/website/catalog.xml content/schema/docbook/catalog.xml"

rm content/$POLANG/autolayout.xml
xsltproc --output content/$POLANG/autolayout.xml docbook-xsl/website/xsl/autolayout.xsl content/$POLANG/layout.xml 
xsltproc --nonet --path "content/$POLANG/" --stringparam output-root website-generated/$POLANG/ --stringparam page-language $l bibletime-xsl/bibletime.xsl content/$POLANG/autolayout.xml
cp -Raf website-common/* website-generated/
$(cd website-generated/postats && ./make_postats.pl);

done;
