#!/usr/bin/perl -w

#
# This script is supposed to generate a database with OSIS scheme information
# as well as mapping data from CCEL/SIL/OSIS.
#
# Author: Martin Gruner, mgruner@crosswire.org
# License: GPL
#

use strict;
use utf8;
use DBI;
use DBD::SQLite; #make sure it is installed, we won't use it directly

my (@mappings, @bible, @bible_nab, @bible_njb, @bible_org, @bible_vul, @bible_lxx);

my $version = 0.1;

##########################
#LOAD DATA
##########################
sub loadFromFile
{
	my $filename = shift;
	my $arrayref = shift;
	open(FH, $filename) or die "can't open $filename\n";
	@{$arrayref} = <FH>;
	close(FH);
}

##########################
#INIT DBs
##########################
unlink "v11n-mapper.db";
my $dbh_mapper = DBI->connect("dbi:SQLite:dbname=v11n-mapper.db","","") || die "can't connect to SQLite database\n";
$dbh_mapper->{unicode} = 1;
$dbh_mapper->{AutoCommit} = 0;  # enable transactions
$dbh_mapper->{RaiseError} = 1;
unlink "v11n-schema.db";
my $dbh_schema = DBI->connect("dbi:SQLite:dbname=v11n-schema.db","","") || die "can't connect to SQLite database\n";
$dbh_schema->{unicode} = 1;
$dbh_schema->{AutoCommit} = 0;  # enable transactions
$dbh_schema->{RaiseError} = 1;

#store version info
foreach my $dbh ($dbh_mapper, $dbh_schema)
{
	$dbh->do("CREATE TABLE version (version TEXT NOT NULL UNIQUE)") || die $!;
	$dbh->do("INSERT INTO version VALUES ($version)") || die $!;
	$dbh->commit();
}

sub feedSchema
{
	my $dbh = shift;
	my $table_name = shift;
	my $array_ref = shift;

	print "Feeding schema $table_name into DB.\n";

	$dbh->do("CREATE TABLE $table_name (osisID TEXT NOT NULL UNIQUE)") || die $!;
	
	my ($osisID, %osisIDs_in_DB);
	my @lines = grep(m/<osisID code/, @{$array_ref});
	die "no lines available!\n" unless @lines;

	foreach my $osisID_line (@lines)
	{
		($osisID) = $osisID_line =~ m/code="(.+)"/;
		next if (exists($osisIDs_in_DB{$osisID})); #keep osisID column unique
		$dbh->do("INSERT INTO $table_name VALUES (\'$osisID\')") || die "$!\n";
		$osisIDs_in_DB{$osisID} = 1;
	}
	$dbh->commit();
}

sub feedMapping
{
	my $dbh = shift;
	my $scheme1 = shift;
	my $source = shift;
	my $scheme2 = shift;
	my $target = shift;
	my $array_ref = shift;

	print "Feeding mapping \"$scheme1 to $scheme2\" into DB.\n";

	$dbh->do("CREATE TABLE ".$source."_to_".$target." (source TEXT NOT NULL UNIQUE, target TEXT NOT NULL)") || die $!;
	
	my ($source_osisID, $target_osisID, %source_osisIDs_in_DB);

	my @lines = grep(m/$scheme1:.+:$scheme2:.+/, @{$array_ref});
	die "no lines available!\n" unless @lines;
	
	foreach my $mapping_line (@lines)
	{
		($source_osisID, $target_osisID) = $mapping_line =~ m/$scheme1:(.+):$scheme2:(.+)/;
		next if ($source_osisID eq $target_osisID); #don't record something that does not need mapping
		next if ( exists($source_osisIDs_in_DB{$source_osisID}) ); #keep source column unique

		$dbh->do("INSERT INTO ".$source."_to_".$target." VALUES (\'$source_osisID\', \'$target_osisID\')") || die "$!\n";
		$source_osisIDs_in_DB{$source_osisID} = 1;
	}
	$dbh->commit();
}

loadFromFile( "data/Bible.xml", 			\@bible );
&feedSchema($dbh_schema, "bible", \@bible);
@bible=();

loadFromFile( "data/Bible.NAB.xml", 	\@bible_nab );
&feedSchema($dbh_schema, "bible_nab", \@bible_nab);
@bible_nab=();

loadFromFile( "data/Bible.NJB.xml", 	\@bible_njb );
&feedSchema($dbh_schema, "bible_njb", \@bible_njb);
@bible_njb=();

loadFromFile( "data/Bible.ORG.xml", 	\@bible_org );
&feedSchema($dbh_schema, "bible_org", \@bible_org);
@bible_org=();

loadFromFile( "data/Bible.Vul.xml", 	\@bible_vul );
&feedSchema($dbh_schema, "bible_vul", \@bible_vul);
@bible_vul=();

loadFromFile( "data/Bible.LXX.xml", 	\@bible_lxx );
&feedSchema($dbh_schema, "bible_lxx", \@bible_lxx);
@bible_lxx=();

loadFromFile( "data/refsysmap.unl", 	\@mappings );
foreach my $scheme1 ( qw(Bible Bible.NAB Bible.NJB Bible.ORG Bible.Vul Bible.LXX) )
{
	foreach my $scheme2 ( qw(Bible Bible.NAB Bible.NJB Bible.ORG Bible.Vul Bible.LXX) )
	{
		next if ($scheme1 eq $scheme2); #no mapping neccessary
		next if (($scheme1 ne "Bible") && ($scheme2 ne "Bible")); #no data available
		(my $source = $scheme1) =~ s/(.*)\.(.*)/$1_$2/;
		(my $target = $scheme2) =~ s/(.*)\.(.*)/$1_$2/;

		&feedMapping( $dbh_mapper, $scheme1, lc($source), $scheme2, lc($target), \@mappings );
	}
}

$dbh_schema->disconnect();
$dbh_mapper->disconnect();

print "Done.\n"