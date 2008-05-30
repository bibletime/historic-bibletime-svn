#!/usr/bin/perl

use lib "Perl";
use Digest::MD5;
use File::Path;
use File::Copy;
use File::Compare;
use Locale::PO;
use strict;

# This script updates the translation in the folder given as parameter

my $TARGET="website-generated";

sub get_langs {
	my $file = shift || "config/language-online.conf";
	
	open(IN, "< $file");
	my $line = <IN>;
	my @langs = split(" ", $line);
	close(IN);
	
	return @langs;
}

sub create_apache_files() {
	my $source = shift;
	my $dest = shift;
	my @langs = &get_langs("config/language-online.conf");
	
	print "Creating apache files ...\n";
	
	#Create the robots.txt file
	open(OUT, "> $dest/robots.txt");
	print OUT "# Robots file for www.bibletime.info. Created by update_translation.pl\n";
	print OUT "User-agent: *\n";
	foreach my $lang (sort @langs) {
		print OUT "Allow: /$lang/\n";
	}
	close(OUT);
	
	#Create the .var files
	opendir(DIR, "$source");
	while (my $file = readdir(DIR)) {
		next unless ($file =~ /\.html|\.shtml|\.phtml|\.php4|\.php$/);
		#print "$file\n";
		
		my $htmlfile = $file;
		$file =~ s/\.html|\.shtml|\.phtml|.php4|.php$/.var/;

		open(OUT, "> $dest/$file");
		
		print OUT "URI: en/$htmlfile\n";
		print OUT "Content-type: text/html\n";
		print OUT "Content-language: en\n";
		
		foreach my $lang (@langs) {
			#print "$lang\n";
			print OUT "\nURI: $lang/$htmlfile\n";
			print OUT "Content-type: text/html\n";
			print OUT "Content-language: $lang\n";
		}
		
		print OUT "\nURI: en/$htmlfile\n";
		print OUT "Content-type: text/html\n";
		
		close(OUT);
	}
	
	closedir(DIR);
}

#Creates the HTML files for the files in the dir given by the first parameter
sub create_html {
	my $dir = shift || die;
	my $dirOut = shift || die;
	my $lang = shift || die;

	print "Creating html files for $dir in $dirOut\n";

	unlink "$dir/autolayout.xml";

	my $command_layout = "XML_CATALOG_FILES='docbook-xsl/catalog.xml docbook-xsl/website/catalog.xml' xsltproc --output $dir/autolayout.xml docbook-xsl/website/xsl/autolayout.xsl --stringparam page-language $lang $dir/layout.xml";

	my $command_create = "XML_CATALOG_FILES='docbook-xsl/catalog.xml docbook-xsl/website/catalog.xml' xsltproc --nonet --path $dir --stringparam output-root $dirOut --stringparam page-language $lang bibletime-xsl/bibletime.xsl $dir/autolayout.xml";

	print "$command_layout\n";
	`$command_layout`;
	print "$command_create\n";
	`$command_create`;
}

# Either the parameters or the languages we know
my @langs;
while (my $lang = pop(@ARGV)) {
	push(@langs, $lang);
}

if (!@langs) {
	@langs = sort &get_langs();
	#("bg", "cs", "de", "fr", "nl", "ko", "pt-br", "ro", "ru", "ua");
}


&create_apache_files($ENV{"PWD"} . "/$TARGET/en", "$TARGET");



