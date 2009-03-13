<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xweb="xalan://com.nwalsh.xalan.Website"
                xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
                exclude-result-prefixes="sweb xweb" version="1.0">

    <!-- ================================================= -->
    <xsl:template name="webpage.footer">
        <xsl:variable name="page" select="." />
        <ul id="footer">
            <li class="footcopy">
                <xsl:choose>
                    <xsl:when test="head/copyright">
                        <xsl:apply-templates select="head/copyright"
                                             mode="titlepage.mode" />
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:apply-templates mode="titlepage.mode"
                                             select="$autolayout/autolayout/copyright" />
                    </xsl:otherwise>
                </xsl:choose>
            </li>

            <xsl:variable name="id">
                <xsl:call-template name="object.id" />
            </xsl:variable>
            <xsl:variable name="entry" select="$autolayout//*[@id=$id]" />
            <xsl:variable name="htmlfilename" select="concat($entry/@dir,$entry/@filename)" />

            <li>
                <xsl:choose>
                    <xsl:when test="$l10n.gentext.default.language = 'de'">
                        <a href="impressum.html">Impressum</a>
                    </xsl:when>
                    <xsl:otherwise>
                        <a href="impressum.html">Imprint</a>
                    </xsl:otherwise>
                </xsl:choose>
            </li>

            <!--<li id="flags">-->
                <!--<xsl:choose>-->
                    <!--<xsl:when test="$l10n.gentext.default.language = 'de'">-->
                        <!--<a href="../en/{$htmlfilename}">-->
                            <!--<img src="../images/flags/en.png" alt="This page in English" width="19" height="13" />-->
                        <!--</a>-->
                    <!--</xsl:when>-->
                    <!--<xsl:otherwise>-->
                        <!--<a href="../de/{$htmlfilename}">-->
                            <!--<img src="../images/flags/de.png" alt="Diese Seite in Deutsch" width="19" height="13" />-->
                        <!--</a>-->
                    <!--</xsl:otherwise>-->
                <!--</xsl:choose>-->
            <!--</li>-->
        </ul>
    </xsl:template>

    <xsl:template match="copyright" mode="titlepage.mode">
        <xsl:call-template name="gentext">
            <xsl:with-param name="key" select="'Copyright'" />
        </xsl:call-template>
        <xsl:call-template name="gentext.space" />
        <xsl:call-template name="dingbat">
            <xsl:with-param name="dingbat">copyright</xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="gentext.space" />
        <xsl:call-template name="copyright.years">
            <xsl:with-param name="years" select="year" />
            <xsl:with-param name="print.ranges"
                            select="$make.year.ranges" />
            <xsl:with-param name="single.year.ranges"
                            select="$make.single.year.ranges" />
        </xsl:call-template>
        <xsl:call-template name="gentext.space" />
        <xsl:apply-templates select="holder" mode="titlepage.mode" />
    </xsl:template>
    <!-- ================================================= -->
</xsl:stylesheet>
