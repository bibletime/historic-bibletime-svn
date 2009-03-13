<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xweb="xalan://com.nwalsh.xalan.Website"
                xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
                exclude-result-prefixes="sweb xweb" version="1.0">

    <xsl:import href="../docbook-xsl/website/xsl/autolayout.xsl"/>

    <xsl:template name="tocentry">
        <xsl:if test="@revisionflag">
            <xsl:attribute name="revisionflag">
                <xsl:value-of select="@revisionflag"/>
            </xsl:attribute>
        </xsl:if>
        <!--Insert priority attribute-->
        <xsl:if test="@priority">
            <xsl:attribute name="priority">
                <xsl:value-of select="@priority"/>
            </xsl:attribute>
        </xsl:if>
        <!--Insert priority attribute-->
        <xsl:if test="@toc">
            <xsl:attribute name="toc">
                <xsl:value-of select="@toc"/>
            </xsl:attribute>
        </xsl:if>

        <xsl:choose>
            <xsl:when test="@href">
                <xsl:call-template name="tocentry.href"/>
            </xsl:when>
            <xsl:otherwise>
                <xsl:call-template name="tocentry.page"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
</xsl:stylesheet>