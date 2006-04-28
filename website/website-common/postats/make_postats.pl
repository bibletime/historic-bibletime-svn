#!/usr/bin/perl
# This script processes the makefiles and creates the file which contains the necessary
# entries so the SSI script can create the translation statistics

use lib "Perl";
use lib "../Perl";
use Locale::PO;
use strict;

my $BT_HOME="/home/joachim/Projekte/BibleTime";
my $WEB_HOME="$BT_HOME/bibletime-website2";

sub get_langs {
	my $file = shift || "language.conf";
	
	open(IN, "< $file");
	my $line = <IN>;
	my @langs = split(" ", $line);
	close(IN);
	
	return @langs;
}

sub parse_pofile(){
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

# Create statistics for the given language
sub bibletime_stats() {
	my $sourcedir = shift;
	my $targetfile = shift;
	my @files;

	opendir(DIR, $sourcedir);
	while (my $pofile = readdir(DIR)) {
		next unless ($pofile =~ /(\.po)$/);
		push (@files, $pofile);
	}
	closedir(DIR);

	open(FILE, "> $targetfile");
	foreach my $pofile (sort(@files)) {
		my $lang = $pofile;
		$lang =~ s/(\.po)$//;
		print FILE &parse_pofile("$sourcedir/$pofile", "$lang");
	}
	close(FILE);
}

#website stats
open(OUT, "> website_stats.txt");

my @langs = sort &get_langs("$WEB_HOME/config/language.conf");
foreach my $lang (@langs){
	print OUT &parse_pofile( "$WEB_HOME/content/$lang/po/full.po", "$lang" );
}
close(OUT);

&bibletime_stats( "$BT_HOME/bibletime-i18n/po", 	 "messages_stats.txt" );
&bibletime_stats( "$BT_HOME/bibletime-i18n/po/howto", 	 "howto_stats.txt" );
&bibletime_stats( "$BT_HOME/bibletime-i18n/po/handbook", "handbook_stats.txt" );
