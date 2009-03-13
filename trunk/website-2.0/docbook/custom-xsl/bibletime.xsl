<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xweb="xalan://com.nwalsh.xalan.Website"
	xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
	exclude-result-prefixes="sweb xweb" version="1.0">

	<xsl:import href="../docbook-xsl/website/xsl/website.xsl" />
	<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/xhtml/chunker.xsl" />
	<xsl:include href="../docbook-xsl/website/xsl/chunk-common.xsl" />

	<xsl:include href="bibletime-common.xsl" />
	<xsl:include href="bibletime-head.xsl" />
	<xsl:include href="bibletime-toc.xsl" />
	<xsl:include href="bibletime-footer.xsl" />

    <xsl:output method="xml" indent="no" omit-xml-declaration="no" encoding="UTF-8" />
    <xsl:strip-space elements="*" />

    <!--BibleTime stylesheet params-->
    <xsl:param name="flags.languages">en</xsl:param>
    <!--en cs da de es fi fr nl nn pt-br ko ru-->

    <!-- Customisation layer -->
    <xsl:param name="img.src.path"><xsl:text></xsl:text></xsl:param>
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
    
	<!-- Netscape gets badly confused if it sees a CSS style... -->
	<!--<xsl:param name="admon.style" select="''" />-->
	<!--<xsl:param name="admon.graphics" select="0" />-->
	<!--<xsl:param name="admon.graphics.path">/images/</xsl:param>-->
	<!--<xsl:param name="admon.graphics.extension">.gif</xsl:param>-->

	<!-- ==================================================================== -->

	<!--<xsl:param name="callout.graphics">0</xsl:param>-->

	<!--<xsl:param name="css.decoration">0</xsl:param>-->
	<!--<xsl:param name="use.viewport">0</xsl:param>-->
	<!--<xsl:param name="spacing.paras">0</xsl:param>-->
	<!--<xsl:param name="make.valid.html">1</xsl:param>-->
	<!--<xsl:param name="html.cleanup">1</xsl:param>-->
<!---->
	<xsl:param name="make.single.year.ranges" select="1" />
	<xsl:param name="make.year.ranges" select="1" />

	<!--<xsl:param name="nav.graphics">0</xsl:param>-->

	<!--<xsl:param name="page-language" select="'.'" />-->

	<!--<xsl:variable name="l10n.gentext.language">-->
		<!--<xsl:value-of select="$page-language" />-->
	<!--</xsl:variable>-->
	<!--<xsl:variable name="l10n.gentext.default.language">en</xsl:variable>-->


	<!-- ==================================================================== -->
	<xsl:param name="html.base">http://www.bibletime.info/</xsl:param>
	<!--<xsl:param name="generate.id.attributes" select="0" />-->
	<!-- ==================================================================== -->



	<xsl:template match="webpage">
		<xsl:param name="devotional"
			select="document('devotional.xml', /*)" />
		<xsl:param name="tips"
			select="document('sidebar_tips.xml', /*)" />

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

		<xsl:variable name="tocentry"
			select="$autolayout/autolayout//*[$id=@id]" />
		<xsl:variable name="toc"
			select="($tocentry/ancestor-or-self::toc|$autolayout/autolayout/toc[1])[last()]" />
		<xsl:variable name="pageid">
			<xsl:call-template name="object.id" />
		</xsl:variable>

		<html>
			<xsl:apply-templates select="head" mode="head.mode" />
			<xsl:apply-templates select="config" mode="head.mode" />

			<body>
				<xsl:attribute name="id">
					<xsl:value-of select="$pageid" />
					<xsl:text>Page</xsl:text>
				</xsl:attribute>
				
				<div id="header">
					<a href="http://www.bibletime.info/"><img src="../images/header-logo.png" id="headerlogo" alt="headerlogo" /></a>
					<div id="flags">
						<xsl:call-template name="output-flags">
							<xsl:with-param name="langs" select="$flags.languages"/>
							<xsl:with-param name="htmlfilename" select="$htmlfilename" />
						</xsl:call-template>
					</div>
				</div>

				<div id="content">
					<xsl:apply-templates
						select="child::*[name(.) != 'webpage']" />

					<xsl:call-template name="process.footnotes" />
				</div>

				<div id="rightpanel">
					<div id="navigation">
						<xsl:choose>
							<xsl:when test="$toc">
								<xsl:apply-templates select="$toc">
									<xsl:with-param name="pageid"
										select="@id" />
								</xsl:apply-templates>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</div>

					<xsl:apply-templates select="$devotional"
						mode="intro" />
					<xsl:apply-templates select="$tips" mode="tips" />

					<div id="logos">
<a href="http://sourceforge.net/projects/bibletime"><img src="http://sflogo.sourceforge.net/sflogo.php?group_id=954&amp;type=13" width="120" height="30" border="0" alt="Get BibleTime at sourceforge.net." /></a>
						<a
							href="http://validator.w3.org/check/referer">
							<img
								src="http://www.w3.org/Icons/valid-xhtml10"
								alt="Valid XHTML 1.0" height="31" width="88" />
						</a>
						<br />
						<a
							href="http://jigsaw.w3.org/css-validator/check/referer">
							<img
								src="http://jigsaw.w3.org/css-validator/images/vcss"
								alt="Valid CSS" />
						</a>
						<br />
						<img alt="counter" src="http://www.bibletime.info/cgi-sys/Count.cgi?df=bibletime.dat|display=Counter|ft=0|md=6|frgb=100;139;216|dd=B" />
					</div>
					
					<div id="footer">
						<xsl:call-template name="webpage.footer" />
					</div>
					</div>
					
				<!-- Google analytics tracking -->				
				<script type="text/javascript">
					var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
					document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
				</script>
				<script type="text/javascript">
					var pageTracker = _gat._getTracker("UA-1186927-1");
					pageTracker._initData();
					pageTracker._trackPageview();
				</script>		
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
