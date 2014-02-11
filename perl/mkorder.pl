#!/usr/bin/perl

my ($orifname, $newId, $dateOfs)= @ARGV;
#A: orifname = filename of the original autocoms generated order 
#A: newId = id for the new order

open(O, "$orifname") || die("Can't open '$orifname': $!");
my $oritxt= join("",<O>); close(O);
#A: oritext= text for this order

if ($oritxt =~ /svcOrdNbr\s+\w\w(\w*)/) { $oriId= $1;}
else { die("Can't match id in '$orifname'") }

my $newtxt= $oritxt;
$newtxt =~ s/$oriId/$newId/g;

my ($ss, $mm, $hh, $DD, $MMzeroBased, $YYadd1900)= localtime(time()+$dateOfs*60*60*24);
$newTime= sprintf("%02d%02d%02d", substr($YYadd1900,-2) , ($MMzeroBased+1) , $DD);
$newtxt =~ s/svcOrdDtReq.*/svcOrdDtReq $newTime/g;

print STDERR "ORDER fname=$orifname PATCHED oldId='$oriId' newId='$newId' newTime='$newTime'\n";
#A: newtxt = text for the new order (IDs & date changed)
print $newtxt;
