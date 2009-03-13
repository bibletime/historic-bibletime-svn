<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xweb="xalan://com.nwalsh.xalan.Website"
                xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
                exclude-result-prefixes="sweb xweb" version="1.0">

    <xsl:import href="../docbook-xsl/website/xsl/website.xsl" />
    <xsl:import href="../docbook-xsl/xhtml/chunker.xsl" />
    <xsl:include href="../docbook-xsl/website/xsl/chunk-common.xsl" />

    <xsl:include href="website-common.xsl" />
    <xsl:include href="header.xsl" />
    <xsl:include href="toc.xsl" />
    <xsl:include href="footer.xsl" />

    <xsl:output method="xml" indent="no" omit-xml-declaration="no" encoding="UTF-8" />
    <xsl:strip-space elements="*" />

    <!-- Customisation layer -->
    <xsl:param name="img.src.path">
        <xsl:text>../</xsl:text>
    </xsl:param>
    <xsl:param name="spacing.paras" select="0" />
    <xsl:param name="html.cleanup" select="1" />
    <xsl:param name="generate.id.attributes" select="1" />
    <xsl:param name="css.decoration" select="0" />

    <xsl:param name="formal.object.break.after" select="0" />

    <xsl:param name="formal.title.placement">
        figure before example before equation before table before procedure before task before
    </xsl:param>

    <!--TOC-->
    <xsl:param name="toc.section.depth">4</xsl:param>
    <xsl:param name="generate.section.toc.level">0</xsl:param>

    <xsl:param name="highlight.source" select="1" />

    <xsl:param name="admon.graphics.path">../images/</xsl:param>
    <xsl:param name="admon.graphics" select="1" />

    <xsl:param name="files.scalanewsletter">scalanews-0001.xml scalanews-0002.xml</xsl:param>

    <xsl:param name="files.scala101">scala101-003.xml scala101-002.xml scala101-001.xml</xsl:param>

    <!-- ==================================================================== -->
    <xsl:template match="webpage">
        <xsl:variable name="id">
            <xsl:call-template name="object.id" />
        </xsl:variable>

        <xsl:variable name="relpath">
            <xsl:call-template name="root-rel-path">
                <xsl:with-param name="webpage" select="." />
            </xsl:call-template>
        </xsl:variable>

        <xsl:variable name="entry" select="$autolayout//*[@id=$id]" />
        <xsl:variable name="htmlfilename"
                      select="concat($entry/@dir,$entry/@filename)" />

        <xsl:variable name="filename">
            <xsl:apply-templates select="." mode="filename" />
        </xsl:variable>

        <xsl:variable name="tocentry" select="$autolayout/autolayout//*[$id=@id]" />
        <xsl:variable name="toc"
                      select="($tocentry/ancestor-or-self::toc|$autolayout/autolayout/toc[1])[last()]" />
        <xsl:variable name="pageid">
            <xsl:call-template name="object.id" />
        </xsl:variable>

        <html>
            <xsl:apply-templates select="head" mode="head.mode" />
            <xsl:apply-templates select="config" mode="head.mode" />

            <body lang="{$l10n.gentext.default.language}" xml:lang="{$l10n.gentext.default.language}">
                <div id="page">
                    <xsl:choose>
                        <xsl:when test="$toc">
                            <xsl:apply-templates select="$toc">
                                <xsl:with-param name="pageid" select="@id" />
                            </xsl:apply-templates>
                        </xsl:when>
                        <xsl:otherwise>&#160;</xsl:otherwise>
                    </xsl:choose>

                    <div id="logo">
                        <img src="../images/logo.png" alt="Joachim Ansorg IT-Services" width="240" height="63" />

                        <div id="flags">
                            <xsl:choose>
                                <xsl:when test="$l10n.gentext.default.language = 'de'">
                                    <a href="../en/{$htmlfilename}">
                                        <img src="../images/flags/en.png" alt="This page in English"
                                             title="This page in English" width="19" height="13" />
                                    </a>
                                </xsl:when>
                                <xsl:otherwise>
                                    <a href="../de/{$htmlfilename}">
                                        <img src="../images/flags/de.png" alt="Diese Seite in Deutsch"
                                             title="Diese Seite auf Deutsch" width="19" height="13" />
                                    </a>
                                </xsl:otherwise>
                            </xsl:choose>
                        </div>
                    </div>

                    <div id="content">
                        <xsl:attribute name="class">
                            <xsl:choose>
                                <xsl:when test="section[@id='sidebar'] | section[@role='newsletter'] | section[@role='handybirthdays']">sidebar</xsl:when>
                                <xsl:otherwise>nosidebar</xsl:otherwise>
                            </xsl:choose>
                        </xsl:attribute>

                        <!--Insert the page title as a h1 header-->
                        <xsl:choose>
                            <xsl:when test="./head/title">
                                <xsl:apply-templates select="./head/title" mode="title.mode" />
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:apply-templates select="./head/titleabbrev" mode="title.mode" />
                            </xsl:otherwise>
                        </xsl:choose>

                        <xsl:apply-templates select="child::*[name(.) != 'webpage']" />

                        <xsl:if test="@id='newsletterArchive'">
                            <xsl:call-template name="newsletterArchive">
                                <xsl:with-param name="files" select="$files.scalanewsletter" />
                            </xsl:call-template>
                        </xsl:if>

                        <xsl:if test="@id='scala101Archive'">
                            <xsl:call-template name="newsletterArchive">
                                <xsl:with-param name="files" select="$files.scala101" />
                            </xsl:call-template>
                        </xsl:if>

                        <xsl:call-template name="process.footnotes" />
                    </div>

                    <xsl:apply-templates select="section[@id='sidebar'] | section[@role='newsletter'] | section[@role='handybirthdays']"
                                         mode="sidebar.mode" />
                    <xsl:call-template name="webpage.footer" />
                </div>

                <!-- Google analytics -->
                <script type="text/javascript">
                    var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
                    document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
                </script>
                <script type="text/javascript">
                    try {
                        var pageTracker = _gat._getTracker("UA-6978286-1");
                        pageTracker._trackPageview();
                    } catch(err) {
                    }</script>
            </body>
        </html>
    </xsl:template>

    <xsl:template match="section[@id='sidebar']" mode="sidebar.mode">
        <div id="sidebar">
            <xsl:apply-templates select="./title" mode="title.mode" />
            <xsl:apply-templates />
        </div>
    </xsl:template>

    <xsl:template match="section[@role='newsletter']" mode="sidebar.mode">
        <div id="sidebar">
            <xsl:apply-templates select="document('snippet_subscribe101.xml')/webpage/section/section" />

            <xsl:apply-templates select="/webpage/head/abstract" />

            <h2>
                <xsl:call-template name="gentext">
                    <xsl:with-param name="key">TableofContents</xsl:with-param>
                </xsl:call-template>
            </h2>
            <xsl:call-template name="make.toc">
                <xsl:with-param name="nodes" select="./section" />
                <xsl:with-param name="toc.title.p" select="false()" />
            </xsl:call-template>

            <xsl:call-template name="make.lots">
                <xsl:with-param name="toc.params">example</xsl:with-param>
                <xsl:with-param name="toc" />
            </xsl:call-template>
        </div>
    </xsl:template>

    <!--the subscribe to handybirthdays newsletter-->
    <xsl:template match="section[@role='handybirthdays']" mode="sidebar.mode">
        <div id="sidebar">
            <xsl:apply-templates select="document('snippet_subscribehandybirthdays.xml')/webpage/section/section" />

            <h2>
                <xsl:call-template name="gentext">
                    <xsl:with-param name="key">TableofContents</xsl:with-param>
                </xsl:call-template>
            </h2>
            <xsl:call-template name="make.toc">
                <xsl:with-param name="nodes" select="./section" />
                <xsl:with-param name="toc.title.p" select="false()" />
            </xsl:call-template>
        </div>
    </xsl:template>

    <!--Remove the elements from the toplevel -->
    <xsl:template match="section[@id='sidebar']"></xsl:template>

    <!-- Abstract template -->
    <xsl:template match="abstract">
        <h2>
            <xsl:call-template name="gentext">
                <xsl:with-param name="key">Abstract</xsl:with-param>
            </xsl:call-template>
        </h2>
        <div class="abstract">
            <xsl:apply-templates select="./para" />
        </div>
    </xsl:template>

    <xsl:template match="section/sectioninfo">
        <div class="author">
            <xsl:choose>
                <xsl:when test="$l10n.gentext.default.language = 'de'">von</xsl:when>
                <xsl:otherwise>by</xsl:otherwise>
            </xsl:choose>
            <xsl:value-of select="' '" /><xsl:value-of select="./author" />
        </div>
    </xsl:template>

    <xsl:template match="//section[@id='lastScala101']">
        <xsl:call-template name="newsletterArchive">
            <xsl:with-param name="files" select="$files.scala101" />
            <xsl:with-param name="count" select="3" />
        </xsl:call-template>
    </xsl:template>
</xsl:stylesheet>
