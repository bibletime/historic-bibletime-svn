# This package contains all the functions used to configure the SSI package used to embed
# commonly ued scripts into webpages.

package SSI::Config;

use strict;
use DBI;

# Constructor
sub new {
	my $class = shift;

	my $self = { #hash which holds the configuration
		local => {
			db_source => 'DBI:mysql:db72024670',
			db_user => 'p34040278',
			db_passwd => 'nixGUTda',
			document_root => '/home/joachim/BibleTime/bibletime-website',
		},
		remote => {
			db_source => 'DBI:mysql:joachima_btguest',
			db_user => 'joachima_btguest',
			db_passwd => 'Q_ljxGtDzs',
			document_root => undef,
		},
		db_connection => undef,
	};
	bless($self, $class);
	return $self;
};


sub dbh() {
	my $self = shift;

	unless ( defined $self->{'db_connection'} ) {
		$self->{'db_connection'} =
			DBI->connect(
				$self->isRemote() ? $self->{'remote'}->{'db_source'} 	: $self->{'local'}->{'db_source'},
				$self->isRemote() ? $self->{'remote'}->{'db_user'} 		: $self->{'local'}->{'db_user'},
				$self->isRemote() ? $self->{'remote'}->{'db_passwd'} 	: $self->{'local'}->{'db_passwd'},
			) || die "SSI::Config::BibleTime: database connection failed!\n" . DBI->errstr;
	}

	return $self->{'db_connection'};
};

sub documentRoot() {
	my $self = shift;
	return $ENV{'DOCUMENT_ROOT'} || "";// || $self->{'local'}->{'document_root'};
};

sub isRemote() {
	my $self = shift;

	# return whether we're remote or local
	return ($self->documentRoot() ne $self->{'local'}->{'document_root'});
}


1;
