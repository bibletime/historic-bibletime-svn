#!/bin/bash
export POLANG=en
export XML_CATALOG_FILES="docbook-xsl/catalog.xml docbook-xsl/website/catalog.xml content/schema/docbook/catalog.xml"

rm content/$POLANG/autolayout.xml
xsltproc --output content/$POLANG/autolayout.xml docbook-xsl/website/xsl/autolayout.xsl content/$POLANG/layout.xml 
xsltproc --nonet --path "content/$POLANG/" --stringparam output-root website-generated/$POLANG/ bibletime-xsl/bibletime.xsl content/$POLANG/autolayout.xml
cp -Raf website-common/* website-generated/
$(cd website-generated/postats && ./make_postats.pl)
