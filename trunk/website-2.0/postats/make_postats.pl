#!/usr/bin/perl -w
# This script processes the makefiles and creates the file which contains the necessary
# entries so the SSI script can create the translation statistics

use lib "./Perl";
use lib "./postats/Perl";
use Locale::PO;
use strict;

sub parse_pofile {
	my $pofile = shift;
	my $lang   = shift;

	my $translated = 0;
	my $untranslated = 0;
	my $fuzzy = 0;

	my $aref = Locale::PO->load_file_asarray("$pofile");
	my @entries = @$aref if ($aref);
	if ($#entries+1) {
		foreach my $entry (@entries) {
			my $msgid = $entry->dequote( $entry->msgid() );
			my $msgstr = $entry->dequote( $entry->msgstr() );

			my $no_msgid = !$msgid || ($msgid eq "") || (length($msgid) == 0);
			my $no_msgstr = !$msgstr || ($msgstr eq "") || (length($msgstr) == 0);

			if ($no_msgid) {
				next;
			}

			if ($no_msgstr) {
				++$untranslated;
			}
			elsif ($entry->fuzzy()) {
				++$fuzzy;
			}
			else {
				++$translated;
			}
		}
	}
	return "$lang \t$translated \t$untranslated \t$fuzzy\n";
}

sub printUsage() {
  print "\nUsage:\n\tmake_postats.pl file.po langugagename\n\tFor example:\tmake_postats.pl i18n/de.po de\n\n"
}

#website stats

if (!-f $ARGV[0] || $ARGV[1] eq '') { 
  printUsage();
  exit -1;
}

print &parse_pofile($ARGV[0], $ARGV[1]);


