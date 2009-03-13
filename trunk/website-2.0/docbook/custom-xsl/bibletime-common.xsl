<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xweb="xalan://com.nwalsh.xalan.Website"
	xmlns:sweb="http://nwalsh.com/xslt/ext/com.nwalsh.saxon.Website"
	exclude-result-prefixes="sweb xweb" version="1.0">


	<xsl:template match="ssi">
		<xsl:variable name="page" select="." />
		<xsl:variable name="mode" select="$page/@mode" />

		<xsl:choose>
			<xsl:when test="$mode='exec'">
				<xsl:variable name="cgi" select="$page/@cgi" />
				<xsl:variable name="exec" select="$page/@cmd" />
				<xsl:choose>
					<xsl:when test="$cgi != ''">
						<xsl:comment>#exec cgi="<xsl:value-of select="$cgi" />"</xsl:comment>
					</xsl:when>
					<xsl:when test="$exec != ''">
						<xsl:comment>#exec cmd="<xsl:value-of select="$exec" />"</xsl:comment>
					</xsl:when>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$mode='include'">
				<xsl:variable name="file" select="$page/@file" />
				<xsl:variable name="virtual" select="$page/@virtual" />
				<xsl:choose>
					<xsl:when test="$file != ''">
						<xsl:comment>#virtual file="<xsl:value-of select="$file" />"</xsl:comment>
					</xsl:when>
					<xsl:when test="$virtual != ''">
						<xsl:comment>#include virtual="<xsl:value-of select="$virtual" />"</xsl:comment>
					</xsl:when>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise></xsl:otherwise>
		</xsl:choose>

	</xsl:template>


	<xsl:template name="print-flag">
		<xsl:param name="lang" />
		<xsl:param name="htmlfilename" />

		<a>
			<xsl:attribute name="title">
				<xsl:value-of select="$lang" />
			</xsl:attribute>

			<xsl:attribute name="href">
				<xsl:text>../</xsl:text>
				<xsl:value-of select="$lang" />
				<xsl:text>/</xsl:text>
				<xsl:value-of select="$htmlfilename" />
			</xsl:attribute>

			<img>
				<xsl:attribute name="alt"><xsl:value-of select="$lang" /></xsl:attribute>

				<xsl:attribute name="src">
					<xsl:text>../images/flags/</xsl:text>
					<xsl:value-of select="$lang" />
                    <xsl:text>.png</xsl:text>
				</xsl:attribute>

			</img>
		</a>
	</xsl:template>

	<xsl:template name="output-flags">
		<xsl:param name="langs" />
		<xsl:param name="htmlfilename" />

		<xsl:choose>
			<xsl:when test="string-length(normalize-space( substring-before($langs, ' ') )) &gt; 0">
				<xsl:call-template name="print-flag">
					<xsl:with-param name="lang" select="substring-before(normalize-space($langs), ' ')" />
					<xsl:with-param name="htmlfilename" select="$htmlfilename" />
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="string-length(normalize-space($langs)) &gt; 0">
				<xsl:call-template name="print-flag">
					<xsl:with-param name="lang" select="normalize-space($langs)" />
					<xsl:with-param name="htmlfilename" select="$htmlfilename" />
				</xsl:call-template>
			</xsl:when>

			<xsl:otherwise></xsl:otherwise>
		</xsl:choose>

		<xsl:if test="string-length(normalize-space(substring-after($langs, ' '))) &gt; 0">
			<xsl:call-template name="output-flags">
				<xsl:with-param name="langs" select="substring-after($langs, ' ')" />
				<xsl:with-param name="htmlfilename" select="$htmlfilename" />
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- Called with the devotional file to display the short intro on the right -->
	<xsl:template match="id('devotional')" mode="intro">
		<div id="devotional">
			<h1>
				<xsl:value-of select="./child::section[position()=1]/title" />
			</h1>

			<xsl:apply-templates select="./child::section[position()=1]/para[position()=1]" />

			<xsl:if test="count(./child::section[position()=1]/para) &gt; 1">
				<p>
					[
					<a href="devotional.html">Read more...</a>
					]
				</p>
			</xsl:if>
		</div>
	</xsl:template>

	<!-- Called with the sidebar_tips file to display the short intro on the right -->
	<xsl:template match="id('sidebar_tips')" mode="tips">
		<div class="tip">
			<xsl:for-each select="./child::section">
				<h1>
					<xsl:value-of select="./title" />
				</h1>

				<xsl:apply-templates select="./para" />
			</xsl:for-each>


		</div>
	</xsl:template>

	<!-- ================================================= -->
	<xsl:template match="copyright" mode="footer.mode">
		<p class="{name(.)}">
			<xsl:call-template name="gentext">
				<xsl:with-param name="key" select="'Copyright'" />
			</xsl:call-template>
			<xsl:call-template name="gentext.space" />
			<xsl:call-template name="dingbat">
				<xsl:with-param name="dingbat">
					copyright
				</xsl:with-param>
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
		</p>
	</xsl:template>

	<xsl:template match="year" mode="footer.mode">
		<xsl:apply-templates />
		<xsl:text>,</xsl:text>
	</xsl:template>

	<xsl:template match="year[position()=last()]" mode="footer.mode">
		<xsl:apply-templates />
	</xsl:template>

	<xsl:template match="holder" mode="footer.mode">
		<xsl:apply-templates />
		<xsl:if test="position() != last()">,</xsl:if>
	</xsl:template>
	<!-- ================================================= -->


	<!-- ================================================= -->
	<xsl:template match="formalpara">
		<xsl:call-template name="paragraph">
			<xsl:with-param name="class">
				<xsl:if test="@role and $para.propagates.style != 0">
					<xsl:value-of select="@role" />
				</xsl:if>
			</xsl:with-param>
			<xsl:with-param name="content">
				<xsl:call-template name="anchor" />
				<div class="formalpara">
					<xsl:apply-templates />
				</div>
			</xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match="formalpara/title">
		<xsl:variable name="titleStr">
			<xsl:apply-templates />
		</xsl:variable>
		<xsl:variable name="lastChar">
			<xsl:if test="$titleStr != ''">
				<xsl:value-of
					select="substring($titleStr,string-length($titleStr),1)" />
			</xsl:if>
		</xsl:variable>
		<div class="paratitle">
			<xsl:copy-of select="$titleStr" />
			<xsl:if
				test="$lastChar != '' and not(contains($runinhead.title.end.punct, $lastChar))">
				<xsl:value-of
					select="$runinhead.default.title.end.punct" />
			</xsl:if>
			<xsl:text>&#160;</xsl:text>
		</div>
	</xsl:template>

	<xsl:template match="formalpara/para">
		<p>
			<xsl:apply-templates />
		</p>
	</xsl:template>
	
	<!-- ================================================= -->
	
	<xsl:template match="command" mode="pre.mode">
	  <div class="command"><xsl:call-template name="inline.boldseq"/></div>
	</xsl:template>
	
	<xsl:template match="programlisting|screen|synopsis">
	  <xsl:param name="suppress-numbers" select="'0'"/>
	  <xsl:variable name="id">
	    <xsl:call-template name="object.id"/>
	  </xsl:variable>
	
	  <xsl:call-template name="anchor"/>
	
	  <xsl:if test="$shade.verbatim != 0">
	    <xsl:message>
	      <xsl:text>The shade.verbatim parameter is deprecated. </xsl:text>
	      <xsl:text>Use CSS instead,</xsl:text>
	    </xsl:message>
	    <xsl:message>
	      <xsl:text>for example: pre.</xsl:text>
	      <xsl:value-of select="local-name(.)"/>
	      <xsl:text> { background-color: #E0E0E0; }</xsl:text>
	    </xsl:message>
	  </xsl:if>
	
	  <xsl:choose>
	    <xsl:when test="$suppress-numbers = '0' and @linenumbering = 'numbered' and $use.extensions != '0' and $linenumbering.extension != '0'">
	      <xsl:variable name="rtf">
	        <xsl:apply-templates/>
	      </xsl:variable>
	      <pre class="{name(.)}">
	        <xsl:call-template name="number.rtf.lines">
	          <xsl:with-param name="rtf" select="$rtf"/>
	        </xsl:call-template>
	      </pre>
	    </xsl:when>
	    <xsl:otherwise>
	      <div class="{name(.)}">
	        <xsl:apply-templates mode="pre.mode"/>
	      </div>
	    </xsl:otherwise>
	  </xsl:choose>
	</xsl:template>

<!-- Link template. Changed to include a translated title attribute. -->
	<xsl:template name="link.to.page">
	  <xsl:param name="href" select="''"/>
	  <xsl:param name="frompage"/>
	  <xsl:param name="page" select="ancestor-or-self::tocentry"/>
	  
	  <xsl:param name="relpath">
	    <xsl:choose>
	      <xsl:when test="$frompage">
	        <xsl:call-template name="toc-rel-path">
	          <xsl:with-param name="pageid" select="$frompage/@id"/>
	        </xsl:call-template>
	      </xsl:when>
	      <xsl:otherwise>
	        <xsl:call-template name="toc-rel-path">
	          <xsl:with-param name="pageid" select="$page/@id"/>
	        </xsl:call-template>
	      </xsl:otherwise>
	    </xsl:choose>
	  </xsl:param>
	  <xsl:param name="linktext" select="'???'"/>
	
	  <a>
	    <xsl:attribute name="href">
	      <xsl:call-template name="page.uri">
	        <xsl:with-param name="href" select="$href"/>
	        <xsl:with-param name="page" select="$page"/>
	        <xsl:with-param name="relpath" select="$relpath"/>
	      </xsl:call-template>
	    </xsl:attribute>
	    <xsl:if test="summary">
	      <xsl:attribute name="title">
	        <xsl:value-of select="normalize-space(string(summary))"/>
	      </xsl:attribute>
	    </xsl:if>
	    <xsl:copy-of select="$linktext"/>
	  </a>
	</xsl:template>

</xsl:stylesheet>
