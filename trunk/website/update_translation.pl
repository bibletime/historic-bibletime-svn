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

sub filesAreDifferent {
	my $file1 = shift;
	my $file2 = shift;
	
	return compare($file1, $file2);
}

sub get_langs {
	my $file = shift || "config/language-online.conf";
	
	open(IN, "< $file");
	my $line = <IN>;
	my @langs = split(" ", $line);
	close(IN);
	
	return @langs;
}

# 1st parameter is the dir with the english XML sources
# 2nd parameter is the dir where the POT files should be put
sub update_pot_files() {
	my $source = shift;
	my $dest = shift;

	print "Making POT files\n";
	mkpath($dest);

	unlink("$dest/full.pot");

	opendir(DIR, $source);

	while (my $file = readdir(DIR)) {
		next unless ($file =~ /(\.xml|\.docbook)$/);
		next if ($file eq "catalog.xml");

		my $potfile = $dest . $file;
		$potfile =~ s/(\.xml|\.docbook)$/\.pot/;

		my $command = "xml2pot $source/$file > $potfile";
		`$command`;

		#print "\tCreating temporary POT $potfile\n";
	}

	print "Merging POT files.\n";
	`msgcat --force-po -o $dest/full.pot.cat $dest/*.pot 2>&1 > /dev/null`;
	`rm $dest/*.pot 2>&1 > /dev/null`;
	move("$dest/full.pot.cat","$dest/full.pot");

	closedir(DIR);
}


# 1st parameter is the dir where the POT files are
# 2nd parameter is the dir where the PO files should be put
sub update_po_files() {
	my $potdir = shift;
	my $podir  = shift;

	print "$podir: Creating PO files\n";
	mkpath($podir);

	my $potfile = "$potdir/full.pot";
	my $pofile  = "$podir/full.po";

	if (!-f $pofile) {
		`touch $pofile`;
	}

        #print "msgmerge --force-po --no-wrap -o $pofile.new $pofile $potfile\n";


	`msgmerge --force-po --no-wrap -o $pofile.new $pofile $potfile  `;

	if (compare($pofile, "$pofile.new") != 0) { #different
		move("$pofile.new", $pofile);
		#print "\t\tMerged in changes\n";
	}
	else {
		#print "\t\tSame entries!\n";
	}
	unlink("$pofile.new");

	#print "\n\n";
}

# 1st parameter is the dir with the english sources
# 2nd parameter is the dir where the PO files are
# 3rd parameter is the dir where the XML files should be put
sub make_xml_files() {
	my $source = shift;
	my $podir = shift;
	my $dest = shift;
	my $pofile = "$podir/full.po";

	print "Creating XML files\n";

	# Create a PO file which contains the original english strings as msgstr of empty/untranslated entries
	# This way the files won"t be empty if the translation is misisng
	copy("$pofile","$pofile-complete");
	
	my $aref = Locale::PO->load_file_asarray("$pofile");
	my @entries = @$aref if ($aref);
	if ($#entries+1) {
		foreach my $entry (@entries) {
			my $msgid = $entry->dequote( $entry->msgid() );
			my $msgstr = $entry->dequote( $entry->msgstr() );
			my $no_msgstr = !$msgstr || ($msgstr eq "") || (length($msgstr) == 0);

			$entry->msgstr($msgid) if ($msgid && ($no_msgstr || $entry->fuzzy()));
		}
	}
	die unless Locale::PO->save_file_fromarray("$pofile-complete", \@entries);


	opendir(DIR, $source);
	while (my $file = readdir(DIR)) {
		next unless ($file =~ /(\.xml|\.docbook)$/);
		next if ($file eq "catalog.xml");

		my $age_en = -M "$source/$file";
		my $age_translated = -M "$dest/$file";
		my $age_po = -M "$pofile-complete";
		
		#print "no recreateion of $file\n" if (!$alwaysRecreate{"$file"});
		
		next if (-f "$source/$file" && -f "$dest/$file" && ($age_en > $age_translated) && ($age_po > $age_translated)); #english file is older than the translated file so no update needed
		

		#print "\tChecking if we need to create $dest/$file\n";
		print "\tCreating file $dest/$file\n";
		`po2xml $source/$file $pofile-complete > $dest/$file.test`;
		
		
		#if the created XML file is different from the original XML file the PO file had some changes
		#if (($file eq "biblehowto.docbook") || ( ($age_po <= $age_translated) && (filesAreDifferent("$dest/$file", "$dest/$file.test"))) ) {
		#	print "\t\tRecreating $file\n";
			move("$dest/$file.test", "$dest/$file");
		#}
		#else { #the files are the same, remove the test file
		#	unlink("$dest/$file.test");
		#}
		
	}

	# Delete temporary PO file used for XML file creation
	unlink("$pofile-complete");

	print "\n\n";
	closedir(DIR);
}

sub create_apache_files() {
	my $source = shift;
	my $dest = shift;
	my @langs = &get_langs("config/language-online.conf");
	
	print "Creating apache files ...\n";
	
	#Create the robots.txt file
	open(OUT, "> $dest/robots.txt");
	print OUT "# Robots file for www.bibletime.info. Created by update_translation.pl\n";
	print OUT "User-agent *\n";
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

#required for all languages

&update_pot_files($ENV{"PWD"} . "/content/en",  $ENV{"PWD"} . "/content/en/pot/");

foreach my $lang (&get_langs("config/language.conf")) {
	print "Creating PO files for $lang...\n";
	&update_po_files($ENV{"PWD"} . "/content/en/pot/", $ENV{"PWD"} . "/content/$lang/po/");
};

while (my $lang = shift(@langs)) {
	if ($lang eq "en") {
		next;
	}

	print "Working on $lang ...\n";
	&make_xml_files($ENV{"PWD"}. "/content/en", $ENV{"PWD"} . "/content/$lang/po/", $ENV{"PWD"} . "/content/$lang/");
	&create_html("./content/$lang", "./$TARGET/$lang", "$lang");
}

#copy all common files
`cp -raf website-common/* website-common/.htaccess  $TARGET`;

#Copy all language po file
`test -d website-generated/po || mkdir website-generated/po;`;
`for d in \$(cat config/language.conf); do echo "\$d"; cp -Rf content/\$d/po/full.po website-generated/po/bibletime_website_\$d.po; done;`;
`cp content/en/pot/full.pot website-generated/po/bibletime_website_template.pot`;

&create_apache_files($ENV{"PWD"} . "/$TARGET/en", "$TARGET");

#Now update the statistics. At the end so we get all changed to the PO files.


`(cd $TARGET/postats && perl make_postats.pl > /dev/null 2>&1; cd ..)`;

