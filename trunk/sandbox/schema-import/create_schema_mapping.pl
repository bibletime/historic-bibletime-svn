#!/usr/bin/perl

use strict;
use warnings;

use IO::File;
use Data::Dumper;
use DBI;
use DBD::SQLite;

my $fh = new IO::File;
if ( !$fh->open("< data/refsysmap.unl") ) {
	die "Cannot open file 'data/refsysmap.unl'.";
}

unlink "v11n_schema_mapping.sqlite";

my $dbh = DBI->connect("dbi:SQLite:dbname=v11n_schema_mapping.sqlite","","");

my %CCEL2CrosswireSchema = (
	'Bible'     => 'KJV',
	'Bible.Vul' => 'Vulg',         # Vulgate
	'Bible.ORG' => 'Leningrad',    # WLC, UBS

	#    'Bible.LXX' => ''  # Septuagint
	#    'Bible.NAB' => '', # New American Bible
	#    'Bible.NJB' => '', # New Jerusalem Bible
);

my $CreateSQL =<<'EOF';
CREATE TABLE v11n_schema_mapping (
    from_schema  TEXT COLLATE BINARY,
    from_osis_id TEXT COLLATE BINARY,
    to_schema    TEXT COLLATE BINARY,
    to_osis_id   TEXT COLLATE BINARY
);
EOF

$dbh->do($CreateSQL);

$CreateSQL =<<'EOF';
CREATE UNIQUE INDEX v11n_schema_mapping_source_index
    ON v11n_schema_mapping (from_schema, from_osis_id);
EOF
$dbh->do($CreateSQL);

$CreateSQL =<<'EOF';
CREATE INDEX v11n_schema_mapping_from_osis_id_index
    ON v11n_schema_mapping (from_osis_id);

EOF
$dbh->do($CreateSQL);

my $Count = 0;
while ( my $Line = $fh->getline() ) {
	my ( $From, $OsisRefFrom, $To, $OsisRefTo ) =
	    $Line =~ m/^ (Bible[^:]*) : ([^:]+) : (Bible[^:]*) : ([^:\n]+)/mx;
	if ( $OsisRefFrom ne $OsisRefTo ) {
#		print "$From, $OsisRefFrom, $To, $OsisRefTo\n";
		$Count++;
	}
}
print "Total: $Count\n";

$dbh->disconnect();
$fh->close();
