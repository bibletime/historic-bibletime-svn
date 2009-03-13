<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:xweb="xalan://com.nwalsh.xalan.Website"
xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
exclude-result-prefixes="sweb xweb" version="1.0">
  <xsl:template match="title" mode="head.mode">
    <title>BibleTime: 
    <xsl:value-of select="." /> - Open Source bible study software</title>
  </xsl:template>
</xsl:stylesheet>
