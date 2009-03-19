<?xml version="1.0" encoding="UTF-8"?>
<!--
This xsl creates a sitemap for all files. It expects a xml in the form
<languages>
<language name="de" file="de/autolayout.xml"/>
</languages>
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" indent="yes" omit-xml-declaration="no" encoding="UTF-8"/>

    <xsl:param name="url.base">http://www.bibletime.info</xsl:param>
    <xsl:param name="change.date"></xsl:param>
    <xsl:param name="change.freq">weekly</xsl:param>

    <xsl:template match="languages">
        <urlset xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                xsi:schemaLocation="http://www.sitemaps.org/schemas/sitemap/0.9 http://www.sitemaps.org/schemas/sitemap/0.9/sitemap.xsd"
                xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">

            <xsl:for-each select="./language">
                <xsl:variable name="name" select="@name"/>
                <xsl:variable name="doc" select="document(@file)"/>

                <xsl:apply-templates select="$doc/autolayout/copyright" mode="mode.remove"/>
                <xsl:apply-templates select="$doc/autolayout//tocentry|$doc/autolayout/notoc" mode="mode.sitemap">
                    <xsl:with-param name="language" select="$name"/>
                </xsl:apply-templates>
            </xsl:for-each>
        </urlset>
    </xsl:template>

    <xsl:template match="copyright|copyright/title|copyright/holder" mode="mode.remove"></xsl:template>

    <xsl:template match="title|tocentry/title|copyright" mode="mode.sitemap"></xsl:template>
    <xsl:template match="tocentry" mode="mode.sitemap">
        <xsl:param name="language"/>
        <xsl:call-template name="mkToc">
            <xsl:with-param name="language" select="$language"/>
        </xsl:call-template>
    </xsl:template>

    <xsl:template match="notoc" mode="mode.sitemap">
        <xsl:param name="language"/>
        <xsl:if test="@toc = 'true'">
            <xsl:call-template name="mkToc">
                <xsl:with-param name="language" select="$language"/>
            </xsl:call-template>
        </xsl:if>
    </xsl:template>

    <xsl:template name="mkToc">
        <xsl:param name="language"/>
        <xsl:variable name="htmlfile" select="@filename"/>
        <xsl:variable name="priority" select="@priority"/>

        <url xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">
            <loc>
                <xsl:value-of select="$url.base"/>/<xsl:value-of select="$language"/>/<xsl:value-of select="$htmlfile"/>
            </loc>
            <xsl:if test="$priority">
                <priority><xsl:value-of select="$priority"/></priority>
            </xsl:if>
            <xsl:if test="$change.date">
                <lastmod><xsl:value-of select="$change.date"/></lastmod>
            </xsl:if>
            <xsl:if test="$change.freq">
                <changefreq><xsl:value-of select="$change.freq"/></changefreq>
            </xsl:if>
        </url>
    </xsl:template>
</xsl:stylesheet>
