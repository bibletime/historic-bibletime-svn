#!/usr/bin/perl -w
# This script processes the makefiles and creates the file which contains the necessary
# entries so the SSI script can create the translation statistics

use lib "Perl";
use lib "../Perl";
use Locale::PO;
use strict;

my $BT_HOME="/home/jansorg/Projekte/C++/BibleTime";
my $WEB_HOME="$BT_HOME/bibletime-website";

sub get_langs {
	my $file = shift || "language.conf";
	
	open(IN, "< $file");
	my $line = <IN>;
	my @langs = split(" ", $line);
	close(IN);
	
	return @langs;
}

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

sub parse_tsfile {
	print "WOrking on ts file...\n";
	my $file = shift;
	my $lang = shift;

	my ($translated, $untranslated) = (0,0);

	open(IN, "< $file") || return "$lang \t0 \t0 \t0\n";

	while (<IN>) {
		$untranslated++ if ($_ =~ /<translation><\/translation>||<translation type="unfinished">/);	
		$translated++ if ($_ =~ /<translation>.+<\/translation>$/);	
	}

	close(IN);

	return "$lang \t$translated $untranslated \t0\n";
}

# Create statistics for the given language
sub bibletime_stats {
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
	foreach my $file (sort(@files)) {
		my $lang = $file;
		$lang =~ s/(\.po)$//;
		print FILE &parse_pofile("$sourcedir/$file", "$lang");
	}
	close(FILE);
}

sub qt_stats {
	my $sourcedir = shift;
	my $targetfile = shift;
	my @files;

	print "$sourcedir\n";

        opendir(DIR, $sourcedir) || die "Can't open dir $sourcedir";
        while (my $pofile = readdir(DIR)) {
                next unless ($pofile =~ /(\.ts)$/);
                push (@files, $pofile);
        }
        closedir(DIR);

        open(FILE, "> $targetfile") || die "Can't open $targetfile for writing";
        foreach my $pofile (sort(@files)) {
                my $lang = $pofile;
                $lang =~ s/(\.ts)$//;
                print FILE &parse_tsfile("$sourcedir/$pofile", "$lang");
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

&bibletime_stats( "$BT_HOME/bibletime-2.0/i18n/handbook", "handbook_stats.txt" );
&bibletime_stats( "$BT_HOME/bibletime-2.0/i18n/howto", "howto_stats.txt" );

