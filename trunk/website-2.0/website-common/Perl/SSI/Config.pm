# This package contains all the functions used to configure the SSI package used to embed
# commonly ued scripts into webpages.

package SSI::Config;

use strict;
use DBI;

# Constructor
sub new {
	my $class = shift;

	bless($self, $class);
	return $self;
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
