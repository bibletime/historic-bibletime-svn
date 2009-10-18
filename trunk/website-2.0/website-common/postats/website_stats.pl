#!/usr/bin/perl -w


# This is a guestbook application for te XML docbook-xml-website based pages
# The different parts can be embedded online by using SSI calls in the generated HTML pages.
#
# This script should be usable for many situations and different pages.

# Copyright by Joachim Ansorg <joachim@ansorgs.de>

use strict;
use lib '../Perl';
use CGI;
use SSI::POStats;

my $cgi = CGI->new();
my $stats = SSI::POStats->new( $cgi );

print $cgi->header();

print $cgi->start_div({-class=>'postats'});
print $stats->show_stats("website_stats.txt", 'http://bibletime.svn.sourceforge.net/viewvc/*checkout*/bibletime/trunk/website-2.0/i18n/$lang.po', '<a href="/$lang/index.html">$lang</a>');
print $cgi->end_div();
