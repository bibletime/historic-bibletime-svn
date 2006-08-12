#!/usr/bin/perl -w

#
# This script is supposed to demonstrate the usage of the mappings db.
#
# Author: Martin Gruner, mgruner@crosswire.org
# License: GPL
#

use strict;
use utf8;
use DBI;
use DBD::SQLite; #make sure it is installed, we won't use it directly

my $dbh_mapper = DBI->connect("dbi:SQLite:dbname=v11n-mapper.db","","") || die "can't connect to SQLite database\n";
$dbh_mapper->{unicode} = 1;
$dbh_mapper->{AutoCommit} = 0;  # enable transactions
$dbh_mapper->{RaiseError} = 1;

my $source_osisID = shift || "";
my $source_schema = shift || "";
my $target_schema = shift || "";

my @available_schemas = qw(bible bible_nab bible_njb bible_org bible_lxx bible_vul);

sub usageInfo
{
	print(
"Usage: do_mapping.pl <osisID> <source schema> <target schema>
	Where <source schema> and <target schema> may be one of:
		@available_schemas\n");
	print("\n@_\n");
	exit 1;
}

($source_osisID =~ m/\./) || &usageInfo("Please provide a valid osisID.\n");
(grep( $_ eq $source_schema, @available_schemas)) || &usageInfo("Please provide a valid source schema");
(grep( $_ eq $target_schema, @available_schemas)) || &usageInfo("Please provide a valid target schema");
($target_schema ne $source_schema) || &usageInfo("Source and target are identical, no mapping done");

sub do_map
{
	my $osisID = shift;
	my $source = shift;
	my $target = shift;

	#print "SELECT target FROM ".$source."_to_".$target." WHERE source='".$osisID."'\n";
	my $result = @{$dbh_mapper->selectcol_arrayref("SELECT target FROM ".$source."_to_".$target." WHERE source='".$osisID."'")}[0];
	return ($result || $osisID);
}

my $result = "";
if ( ($source_schema ne "bible") && ($target_schema ne "bible") ){
	my $tmp = &do_map($source_osisID, $source_schema, "bible"); #Warning: does not handle ranges
	$result = &do_map($tmp, "bible", $target_schema);
}
else
{
	$result = &do_map($source_osisID, $source_schema, $target_schema);
}
print $result."\n";