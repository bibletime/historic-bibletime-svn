<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml"><head ><meta http-equiv="Content-Type" content="text/html; charset=UTF-8" /><meta name="generator" content="Website XSL Stylesheet V1.0&#10;" /><meta http-equiv="content-language" content="en" /><meta name="DC.Language" content="en" /><link rel="stylesheet" href="blue.css" type="text/css" title="Blue" />
<title>www.bibletime.info: Sword CD order</title>

<meta name="keyword" content="BibleTime, Linux, Software, biblestudy, bible study, KDE, God, Religion, Bible, devotional, crosswire, sword, CD order, Sword CD" />
</head><body id="software_swordcdsend"><div id="pagetop"><img height="99" class="left" alt="pagetop logo" src="/images/pagetop-left.png" /><div id="flags"><a title="en" href="/en/"><img alt="en" src="/images/flags/en.png" /></a><a title="de" href="/de/"><img alt="de" src="/images/flags/de.png" /></a><a title="fr" href="/fr/"><img alt="fr" src="/images/flags/fr.png" /></a><a title="pt-br" href="/pt-br/"><img alt="pt-br" src="/images/flags/pt-br.png" /></a><a title="ko" href="/ko/"><img alt="ko" src="/images/flags/ko.png" /></a><a title="ru" href="/ru/"><img alt="ru" src="/images/flags/ru.png" /></a></div></div><table id="page"><tr><td><div id="navigation"><ul ><li><a href="index.html">Software</a><ul><li><a href="software_about.html">About BibleTime</a></li><li><a href="software_screenshots.html">Screenshots</a></li><li><a href="software_gallery.html">Gallery</a></li><li><a href="software_installation.html">Download</a></li><li><a href="software_faq.html">FAQ</a></li><li><a href="software_links.html">Links</a></li><li><a href="software_linkus.html">Link to us</a></li><li><a href="software_guestbook.shtml">Guestbook</a></li><li><a href="software_press.html">In the press</a></li><li><a href="software_swordcd.shtml">Sword CD</a></li><li><a href="software_contact.html">Contact</a></li><li><a href="software_license.html">License</a></li></ul></li><li><a href="development.html">Development</a><ul><li><a href="development_help.html">Help us</a></li><li><a href="development_join.html">Join us</a></li><li><a href="development_mailinglists.html">Mailinglists</a></li><li><a href="development_team.html">The team</a></li></ul></li><li><a href="translation.shtml">Translation</a></li><li><a href="biblehowto.html">Biblestudy HowTo</a></li></ul><div id="logos"><a href="http://www.sf.net/projects/bibletime/"><img src="http://sourceforge.net/sflogo.php?group_id=954&amp;type=1" alt="sf logo" width="88" height="31" /></a><br /><a href="http://validator.w3.org/check/referer"><img src="http://www.w3.org/Icons/valid-xhtml10" alt="Valid XHTML 1.0" height="31" width="88" /></a><br /><a href="http://jigsaw.w3.org/css-validator/check/referer"><img src="http://jigsaw.w3.org/css-validator/images/vcss" alt="Valid CSS" /></a><br /><img alt="counter" src="http://cgicounter.puretec.de/cgi-bin/cnt?clsid=6bfe60d8ff2e9c56d7db8be538c054421" /></div><div id="copyright"><span id="footcopy"><span  class="copyright">Copyright © 1999, 2000, 2001, 2002, 2003, 2004 The BibleTime developers. </span></span></div></div><div id="content"><div class="section"  xml:lang="en"><div class="titlepage"><div><div><h2 class="title"><a id="id2451570"></a>CD order sent</h2></div></div><div></div></div>

<?php $name 		= $HTTP_POST_VARS["name"];
	$street 		= $HTTP_POST_VARS["street"];
	$town 		= $HTTP_POST_VARS["town"];
	$country 		= $HTTP_POST_VARS["country"];
	$copies 		= $HTTP_POST_VARS["cds"];
	$comments 	= $HTTP_POST_VARS["comments"];
	$email 		= $HTTP_POST_VARS["email"];
	$from 		= "info@bibletime.info";
	
	$subject = "Your Sword CD order";
	$body_confirmation = <<<EOT
Dear $name,
this is the confirmation eMail of your Sword CD order.
Here is the information of your Sword CD order again to give you a reference for later questions you may have.

	Name: $name
	Street: $street
	Town: $town
	Country: $country
	eMail: $email
	Copies: $copies

	Comments: $comments

If you have questions please send an eMail to info\@bibletime.info or simply respond to this eMail!

The BibleTime developers
EOT;

	$body_order = <<<EOT
	Name: $name
	Street: $street
	Town: $town
	Country: $country
	eMail: $email
	Copies: $copies

	Comments: $comments
EOT;

	$headers = "From: info@bibletime.info\r\n";
	$headers .= "Content-type: text/plain; charset=utf-8\r\n";
	
	if ( mail($email, $subject, $body_confirmation, $headers) ) {
		echo("<p>The confirmation eMail was successfully sent to $email.</p>");
	}
	else {
		echo("<p>The confirmation eMail could not be sent to $email.</p>");
	}
	
	if ( mail("info@bibletime.info", "[Sword CD order] $name ordered $copies CD(s)", $body_order, $headers) ) {
		echo("<p>The order was sucessfully send to the BibleTime team.</p>");
	}
	else {
		echo("<p>The order could not be send to the BibleTime team.</p>");
	}
	?> 
</div></div><div id="rightpanel"><div id="devotional"><h3>Wait a minute</h3><p>
Did you see "The Passion"? Whether you found the film inspiring or not, or you didn't see it at all -- If you want to find out more about the passion of Jesus Christ, you can visit the "<a href="http://www.the-good-news.org/" >the good news</a>" webpages. It has more information ready for you, maybe even in your local language.
</p></div><div class="tip"><h3>Translations</h3><p>
It will cost you just a few minutes to update the webpage translation in your language. Help with the translations is something all users can do and which help all users.
If you want to help us keeping the translations up to date, please read the page about 
<a  href="translation.shtml">making translations</a> and update the translation file of your own language.
</p></div></div></td></tr></table></body></html>
