#!/usr/local/bin/perl -s

use PrintRoutines;

my $mibtable  = "\$1.3.6.1.4.1.18.3.3.2.3.1";      # wfCircuitNameEntry
my $nofleafs  = 8;    # no of leaf nodes in the above table (defaults below)
my %leafdefs  = (
		 1 => 1,                           # wfCircuitNameDelete
		 2 => 0,                           # wfCircuitNumber
		 3 => "MCT1-4-2",                  # wfCircuitName
		 4 => 90,                          # wfCircuitIfType (enum type)
		 5 => "\"hex:00:00:00:00:00:00:70:01\"",   # wfCircuitProtoMap
		 6 => 1,                           # wfCircuitType
		 7 => "\"hex:10:04:00:00\"",       # wfCircuitRelCctList
		 8 => "0904102"                    # wfCircuitLineList
		 );

my $idx1limit = 161;
my $lli = 2;

for $cct ( 139 .. $idx1limit )
{
    for $leaf ( 1 .. $nofleafs)
    {
	my $mibval = $leafdefs{$leaf};
	
	if( $leaf eq 3 )
	{
	    $mibval = "\"$leafdefs{$leaf}-$lli\"";
        }
	elsif( $leaf eq 8 )
	{
	    $mibval = "\"$lli$leafdefs{$leaf}\"";
	}
	print "set $mibtable.$leaf.$cct = $mibval\n";
    }
    print "\n";
    $lli++;
}
