<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xweb="xalan://com.nwalsh.xalan.Website"
                xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
                exclude-result-prefixes="sweb xweb" version="1.0">


    <xsl:template match="ssi">
        <xsl:variable name="page" select="."/>
        <xsl:variable name="mode" select="$page/@mode"/>

        <xsl:choose>
            <xsl:when test="$mode='exec'">
                <xsl:variable name="cgi" select="$page/@cgi"/>
                <xsl:variable name="exec" select="$page/@cmd"/>
                <xsl:choose>
                    <xsl:when test="$cgi != ''">
                        <xsl:comment>#exec cgi="<xsl:value-of select="$cgi"/>"
                        </xsl:comment>
                    </xsl:when>
                    <xsl:when test="$exec != ''">
                        <xsl:comment>#exec cmd="<xsl:value-of select="$exec"/>"
                        </xsl:comment>
                    </xsl:when>
                </xsl:choose>
            </xsl:when>
            <xsl:when test="$mode='include'">
                <xsl:variable name="file" select="$page/@file"/>
                <xsl:variable name="virtual" select="$page/@virtual"/>
                <xsl:choose>
                    <xsl:when test="$file != ''">
                        <xsl:comment>#virtual file="<xsl:value-of select="$file"/>"
                        </xsl:comment>
                    </xsl:when>
                    <xsl:when test="$virtual != ''">
                        <xsl:comment>#include virtual="<xsl:value-of select="$virtual"/>"
                        </xsl:comment>
                    </xsl:when>
                </xsl:choose>
            </xsl:when>
            <xsl:otherwise></xsl:otherwise>
        </xsl:choose>

    </xsl:template>

    <!-- ================================================= -->
    <!-- From html.xsl-->
    <xsl:template match="*" mode="class.attribute">
        <xsl:param name="class" select="@role"/>
        <!--xsl:param name="class" select="local-name(.)"/-->
        <!-- permit customization of class attributes -->
        <!-- Use element name by default -->
        <xsl:if test="$class">
            <xsl:attribute name="class">
                <xsl:apply-templates select="." mode="class.value">
                    <xsl:with-param name="class" select="$class"/>
                </xsl:apply-templates>
            </xsl:attribute>
        </xsl:if>
    </xsl:template>

    <!-- ================================================= -->
    <xsl:template name="section.heading">
        <xsl:param name="section" select="."/>
        <xsl:param name="level" select="1"/>
        <!--jansorg: Disabled anchors-->
        <xsl:param name="allow-anchors" select="0"/>
        <xsl:param name="title"/>
        <xsl:param name="class" select="'title'"/>

        <xsl:variable name="id">
            <xsl:choose>
                <!-- if title is in an *info wrapper, get the grandparent -->
                <xsl:when test="contains(local-name(..), 'info')">
                    <xsl:call-template name="object.id">
                        <xsl:with-param name="object" select="../.."/>
                    </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:call-template name="object.id">
                        <xsl:with-param name="object" select=".."/>
                    </xsl:call-template>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>

        <!-- HTML H level is one higher than section level -->
        <xsl:variable name="hlevel">
            <xsl:choose>
                <!-- highest valid HTML H level is H6; so anything nested deeper
         than 5 levels down just becomes H6 -->
                <xsl:when test="$level &gt; 5">6</xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="$level"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>
        <xsl:element name="h{$hlevel}" namespace="http://www.w3.org/1999/xhtml">
            <xsl:attribute name="class">
                <xsl:value-of select="$class"/>
            </xsl:attribute>
            <xsl:if test="$css.decoration != '0'">
                <xsl:if test="$hlevel&lt;3">
                    <xsl:attribute name="style">clear: both</xsl:attribute>
                </xsl:if>
            </xsl:if>
            <xsl:if test="$allow-anchors != 0 and $generate.id.attributes = 0">
                <xsl:call-template name="anchor">
                    <xsl:with-param name="node" select="$section"/>
                    <xsl:with-param name="conditional" select="0"/>
                </xsl:call-template>
            </xsl:if>
            <xsl:if test="$generate.id.attributes != 0 and not(local-name(.) = 'appendix')">
                <xsl:attribute name="id">
                    <xsl:value-of select="$id"/>
                </xsl:attribute>
            </xsl:if>
            <xsl:copy-of select="$title"/>
        </xsl:element>
    </xsl:template>

    <!-- ================================================= -->
    <xsl:template match="blockquote">
        <div class="quote">
            <xsl:apply-templates select="." mode="class.attribute"/>
            <xsl:if test="@lang or @xml:lang">
                <xsl:call-template name="language.attribute"/>
            </xsl:if>
            <xsl:call-template name="anchor"/>
            <xsl:choose>
                <xsl:when test="attribution">
                    <div class="text">
                        <xsl:apply-templates select="child::*[local-name(.)!='attribution']"/>
                    </div>
                    <div class="attribution">
                        <xsl:apply-templates select="attribution"/>
                    </div>
                </xsl:when>
                <xsl:otherwise>
                    <blockquote>
                        <xsl:apply-templates select="." mode="class.attribute"/>
                        <xsl:apply-templates/>
                    </blockquote>
                </xsl:otherwise>
            </xsl:choose>
        </div>
    </xsl:template>

    <!-- ================================================= -->
    <xsl:template match="formalpara/para">
        <xsl:apply-templates/>
        <br/>
    </xsl:template>

    <!-- ================================================= -->
    <xsl:template match="copyright" mode="footer.mode">
        <li class="{name(.)}">
            <xsl:call-template name="gentext">
                <xsl:with-param name="key" select="'Copyright'"/>
            </xsl:call-template>
            <xsl:call-template name="gentext.space"/>
            <xsl:call-template name="dingbat">
                <xsl:with-param name="dingbat">
                    copyright
                </xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="gentext.space"/>
            <xsl:call-template name="copyright.years">
                <xsl:with-param name="years" select="year"/>
                <xsl:with-param name="print.ranges"
                                select="$make.year.ranges"/>
                <xsl:with-param name="single.year.ranges"
                                select="$make.single.year.ranges"/>
            </xsl:call-template>
            <xsl:call-template name="gentext.space"/>
            <xsl:apply-templates select="holder" mode="titlepage.mode"/>
        </li>
    </xsl:template>

    <xsl:template match="year" mode="footer.mode">
        <xsl:apply-templates/>
        <xsl:text>,</xsl:text>
    </xsl:template>

    <xsl:template match="year[position()=last()]" mode="footer.mode">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="holder" mode="footer.mode">
        <xsl:apply-templates/>
        <xsl:if test="position() != last()">,</xsl:if>
    </xsl:template>

    <xsl:template match="abstract" mode="titlepage.mode">
        <div class="abstract">
            <xsl:apply-templates select="." mode="class.attribute"/>
            <xsl:call-template name="anchor"/>
            <xsl:if test="$abstract.notitle.enabled = 0">
                <xsl:call-template name="formal.object.heading">
                    <xsl:with-param name="title">
                        <xsl:apply-templates select="." mode="title.markup"/>
                    </xsl:with-param>
                </xsl:call-template>
            </xsl:if>
            <xsl:apply-templates mode="titlepage.mode"/>
            <xsl:call-template name="process.footnotes"/>
        </div>
    </xsl:template>

    <xsl:template name="olink-file">
        <xsl:param name="xml_filename"/>
        <xsl:param name="content"/>

        <xsl:variable name="xmlfile"
                      select="document($xml_filename,$autolayout)"/>
        <xsl:variable name="webpage"
                      select="$xmlfile/webpage"/>
        <xsl:variable name="tocentry"
                      select="$autolayout//*[$webpage/@id=@id]"/>

        <xsl:variable name="dir">
            <xsl:choose>
                <xsl:when test="starts-with($tocentry/@dir, '/')">
                    <xsl:value-of select="substring($tocentry/@dir, 2)"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="$tocentry/@dir"/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:variable>

        <xsl:choose>
            <xsl:when test="@type = 'embed'">
                <xsl:apply-templates select="$xmlfile"/>
            </xsl:when>
            <xsl:otherwise>
                <!-- @type = 'replace' or @type = 'new' -->
                <a>
                    <xsl:attribute name="href">
                        <xsl:call-template name="root-rel-path"/>
                        <xsl:value-of select="$dir"/>
                        <xsl:value-of select="$filename-prefix"/>
                        <xsl:value-of select="$tocentry/@filename"/>
                        <xsl:if test="@localinfo">
                            <xsl:text>#</xsl:text>
                            <xsl:value-of select="@localinfo"/>
                        </xsl:if>
                    </xsl:attribute>

                    <xsl:if test="@type = 'new'">
                        <xsl:attribute name="target">_blank</xsl:attribute>
                    </xsl:if>

                    <xsl:choose>
                        <xsl:when test="count(node()) = 0">
                            <xsl:apply-templates select="$webpage/head/title"/>
                        </xsl:when>
                        <xsl:when test="$content">
                            <xsl:value-of select="$content"/>
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:apply-templates/>
                        </xsl:otherwise>
                    </xsl:choose>
                </a>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="newsletterArchive">
        <xsl:param name="files" />
        <xsl:param name="count" select="-1"/>

        <ul class="newsletterArchive">
            <xsl:call-template name="recursiveNewsletterAbstracts">
                <xsl:with-param name="files" select="$files"/>
                <xsl:with-param name="count" select="$count"/>
            </xsl:call-template>
        </ul>
    </xsl:template>

    <xsl:template name="recursiveNewsletterAbstracts">
        <xsl:param name="files" />
        <xsl:param name="count" />

        <xsl:if test="(string-length($files) &gt; 0) and not($count = 0)">
            <xsl:choose>
                <xsl:when test="contains($files, ' ')">
                    <!--several files left, process the first, call for the remaining-->
                    <xsl:call-template name="newsletterSummary">
                        <xsl:with-param name="xmlfile" select="substring-before($files, ' ')"/>
                    </xsl:call-template>

                    <xsl:call-template name="recursiveNewsletterAbstracts">
                        <xsl:with-param name="files" select="substring-after($files, ' ')"/>
                        <xsl:with-param name="count" select="$count -1"/>
                    </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                    <!--just one file left-->
                    <xsl:call-template name="newsletterSummary">
                        <xsl:with-param name="xmlfile" select="$files"/>
                    </xsl:call-template>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:if>
    </xsl:template>

    <xsl:template name="newsletterSummary">
        <xsl:param name="xmlfile"/>

        <xsl:variable name="webpage" select="document($xmlfile)/webpage"/>
        <xsl:variable name="tocentry" select="$autolayout//*[webpage/@id=@id]"/>

        <li>
            <xsl:call-template name="olink-file">
                <xsl:with-param name="xml_filename" select="$xmlfile"/>
                <xsl:with-param name="content">
                    <xsl:choose>
                        <xsl:when test="$webpage/head/titleabbrev">
                            <xsl:value-of select="$webpage/head/titleabbrev" />
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:value-of select="$webpage/head/title" />
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:with-param>
            </xsl:call-template>:

            <span><xsl:value-of select="$webpage/head/abstract"/></span>
        </li>
    </xsl:template>


    <xsl:template name="graphical.admonition">
        <xsl:variable name="admon.type">
            <xsl:choose>
                <xsl:when test="local-name(.)='note'">Note</xsl:when>
                <xsl:when test="local-name(.)='warning'">Warning</xsl:when>
                <xsl:when test="local-name(.)='caution'">Caution</xsl:when>
                <xsl:when test="local-name(.)='tip'">Tip</xsl:when>
                <xsl:when test="local-name(.)='important'">Important</xsl:when>
                <xsl:otherwise>Note</xsl:otherwise>
            </xsl:choose>
        </xsl:variable>

        <xsl:variable name="alt">
            <xsl:call-template name="gentext">
                <xsl:with-param name="key" select="$admon.type"/>
            </xsl:call-template>
        </xsl:variable>

        <div>
            <xsl:attribute name="class">
                <xsl:value-of select="$admon.type"/>
            </xsl:attribute>
            <xsl:if test="$admon.style != ''">
                <xsl:attribute name="style">
                    <xsl:value-of select="$admon.style"/>
                </xsl:attribute>
            </xsl:if>

            <table border="0">
                <xsl:attribute name="summary">
                    <xsl:value-of select="$admon.type"/>
                    <xsl:if test="title|info/title">
                        <xsl:text>: </xsl:text>
                        <xsl:value-of select="(title|info/title)[1]"/>
                    </xsl:if>
                </xsl:attribute>
                <tr>
                    <td rowspan="2" align="center" valign="top">
                        <xsl:attribute name="width">
                            <xsl:apply-templates select="." mode="admon.graphic.width"/>
                        </xsl:attribute>
                        <img alt="[{$alt}]">
                            <xsl:attribute name="src">
                                <xsl:call-template name="admon.graphic"/>
                            </xsl:attribute>
                        </img>
                    </td>
                    <th align="left">
                        <xsl:call-template name="anchor"/>
                        <xsl:if test="$admon.textlabel != 0 or title or info/title">
                            <xsl:apply-templates select="." mode="object.title.markup"/>
                        </xsl:if>
                    </th>
                </tr>
                <tr>
                    <td align="left" valign="top">
                        <xsl:apply-templates/>
                    </td>
                </tr>
            </table>
        </div>
    </xsl:template>

    <!-- Fixed for better title formattin-->
    <xsl:template name="list.of.titles">
      <xsl:param name="toc-context" select="."/>
      <xsl:param name="titles" select="'table'"/>
      <xsl:param name="nodes" select=".//table"/>

      <xsl:if test="$nodes">
        <div class="list-of-{$titles}s">
          <h2>
              <xsl:call-template name="gentext">
                <xsl:with-param name="key">
                  <xsl:choose>
                    <xsl:when test="$titles='table'">ListofTables</xsl:when>
                    <xsl:when test="$titles='figure'">ListofFigures</xsl:when>
                    <xsl:when test="$titles='equation'">ListofEquations</xsl:when>
                    <xsl:when test="$titles='example'">ListofExamples</xsl:when>
                    <xsl:when test="$titles='procedure'">ListofProcedures</xsl:when>
                    <xsl:otherwise>ListofUnknown</xsl:otherwise>
                  </xsl:choose>
                </xsl:with-param>
              </xsl:call-template>
          </h2>

          <xsl:element name="{$toc.list.type}" namespace="http://www.w3.org/1999/xhtml">
            <xsl:apply-templates select="$nodes" mode="toc">
              <xsl:with-param name="toc-context" select="$toc-context"/>
            </xsl:apply-templates>
          </xsl:element>
        </div>
      </xsl:if>
    </xsl:template>

    <!-- Example title fixes -->
    <xsl:template name="formal.object.heading">
      <xsl:param name="object" select="."/>
      <xsl:param name="title">
        <xsl:apply-templates select="$object" mode="object.title.markup">
          <xsl:with-param name="allow-anchors" select="1"/>
        </xsl:apply-templates>
      </xsl:param>

      <div class="title"><xsl:copy-of select="$title"/></div>
    </xsl:template>

    <!--Programlistings-->
    <xsl:template match="programlisting|screen|synopsis">
        <xsl:param name="suppress-numbers" select="'0'" />

        <xsl:variable name="id">
            <xsl:call-template name="object.id" />
        </xsl:variable>

        <xsl:call-template name="anchor" />

        <xsl:if test="$shade.verbatim != 0">
            <xsl:message>
                <xsl:text>The shade.verbatim parameter is deprecated. </xsl:text>
                <xsl:text>Use CSS instead,</xsl:text>
            </xsl:message>
            <xsl:message>
                <xsl:text>for example: pre.</xsl:text>
                <xsl:value-of select="local-name(.)" />
                <xsl:text> { background-color: #E0E0E0; }</xsl:text>
            </xsl:message>
        </xsl:if>

        <xsl:choose>
            <xsl:when test="$suppress-numbers = '0' and @linenumbering = 'numbered' and $use.extensions != '0' and $linenumbering.extension != '0'">
                <xsl:variable name="rtf">
                    <xsl:call-template name="apply-highlighting" />
                </xsl:variable>
                <pre>
                    <xsl:apply-templates select="." mode="class.attribute" />
                    <xsl:call-template name="number.rtf.lines">
                        <xsl:with-param name="rtf" select="$rtf" />
                    </xsl:call-template>
                </pre>
            </xsl:when>
            <xsl:otherwise>
                <pre>
                    <xsl:if test="./@lang">
                        <xsl:attribute name="class">brush: <xsl:value-of select="./@lang"/></xsl:attribute>
                    </xsl:if>
                    <xsl:apply-templates select="." mode="class.attribute" />
                    <xsl:call-template name="apply-highlighting" />
                </pre>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet>
