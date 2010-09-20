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
print $stats->show_stats("messages_stats.txt", 'http://gitorious.org/bibletime/bibletime/blobs/raw/master/i18n/messages/bibletime_ui_$lang.ts');
print $cgi->end_div();

