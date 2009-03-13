<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xweb="xalan://com.nwalsh.xalan.Website"
                xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
                exclude-result-prefixes="sweb xweb" version="1.0">

    <xsl:template match="title" mode="head.mode">
        <title><xsl:value-of select="."/> - Joachim Ansorg IT-Services</title>
    </xsl:template>

    <!--Processes keywords. It uses base keywords defined in layout.xml ·-->
    <xsl:template match="keywords" mode="head.mode">
        <xsl:variable name="default_keywords" select="$autolayout/autolayout/config[@param='keywords']/@value"/>
        <xsl:choose>
            <xsl:when test="$default_keywords ">
                <meta name="keyword" content="{$default_keywords}, {.}"/>
            </xsl:when>
            <xsl:otherwise>
                <meta name="keyword" content="{.}"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Added to accept meta tags which are valid for each page -->
    <xsl:template name="user.head.content">
        <xsl:apply-templates select="$autolayout/autolayout/meta" mode="head.mode" />
        <meta http-equiv="content-language" content="{$l10n.gentext.default.language}"/>
        <link rel="icon" href="../images/favicon.png" type="image/png" />
    </xsl:template>
</xsl:stylesheet>
