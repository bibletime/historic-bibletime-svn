<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xweb="xalan://com.nwalsh.xalan.Website"
	xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
	exclude-result-prefixes="sweb xweb" version="1.0">


	<!-- ================================================= -->
	<xsl:template name="webpage.footer">
		<xsl:variable name="page" select="." />

		<div class="footer">
			<span class="footcopy">
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
			</span>
		</div>
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
