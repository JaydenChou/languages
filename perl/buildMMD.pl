#!/usr/local/bin/perl -s
##
## The following perl script takes the perl module/package generated
## by the 'smidump' tool (MibMetaDict.pm) and generated the required
## C++ files for the MIB dictionaries.
##
## The change log is recorded at the bottom of the file.
##

#use strict;
use lib "/home/jfuchs/bin.Perl";
use MibMetaDict;
use snmMIBs;
use PrintRoutines;    # located in /home/jfuchs/bin.Perl

# Dictionaries (hash table) generated. These dictionaries are the key
# to the entire data analysis logic to generate the MibMeta dictionaries.
# All the entries in this dictionary are generated from the 'Dict' hash
# table in the MibMetaDict.pm PERL module.
#
my %oidDict;        # oid -> OID to node mapping
my %nameDict;       # name -> all nodes
my %rowDict;        # name -> row nodes ('xyz'Entry MIBs)
my %colDict;        # name -> col nodes (the leaf attribs under an 'xyz'Entry)
my %enumDict;       # name -> enum nodes
my %enumRowDict;    # name -> enum row nodes
my %enumTypeDict;   # enum-type-name -> enum type nodes
my %snmMIBList;     # collection of MIBs SNM uses

my @arrayEnumStrMap;
my $SYNTAX_OFFSET = 100;

my %uniqueTypes;

my %accessStrMap = (
		    "readonly"    => "READ_ONLY",
		    "readwrite"   => "READ_WRITE",
		    "noaccess"    => "NO_ACCESS",
		    "notifyonly"  => "NO_ACCESS"
		   );

my %typeStrMap = (
		  "<UNKNOWN>"        => "UNKNOWN",
		  "OctetString"      => "OCTETSTRING",
		  "ObjectIdentifier" => "OBJECTIDENTIFIER",
		  "Unsigned32"       => "UNSIGNED32",
		  "Integer32"        => "INTEGER32",
		  "Index"            => "INTEGER32",
		  "Unsigned64"       => "UNSIGNED64",
		  "Integer64"        => "INTEGER64",
		  "Float32"          => "FLOAT32",
		  "Float64"          => "FLOAT64",
		  "Float128"         => "FLOAT128",
		  "Enumeration"      => "ENUMERATION",
		  "Bits"             => "BITS",
		  "entry"            => "ENTRY",
		  "DisplayString"    => "DISPLAYSTRING",
		  "IpAddress"        => "IPADDRESS",
		  "Counter"          => "COUNTER",
		  "Gauge"            => "GAUGE",
		  "NetworkAddress"   => "NETWORKADDRESS",
		  "PhysAddress"      => "PHYSADDRESS",
		  "TimeTicks"        => "TIMETICKS"
		 );

# Setup the logging using the file passed on the command line
# "-log="filename"" option
my $logFile = "";
unless( $log eq "1" )
{
    $logFile = $log;
}
#++ V&V the command line:
## 1. We expect only one argument
## 2. Arg 1 must be one of the the Switch model tags below ...
if( (1 != scalar(@ARGV)) ||
    (($ARGV[0] ne "Bay") && ($ARGV[0] ne "Nortel") && ($ARGV[0] ne "Cisco")) )
{
    print "\n  USAGE: buildMMD.pl <Vendor-Class>\n";
    print "         Vendor-Class = Bay or Nortel or Cisco\n\n";
    exit(0);
}
else
{
    PrintRoutines->new($logFile);
}
#-- V&V complete; Start the processing now ...
print "Building $ARGV[0] MIB dictionary for COLv ...\n";
print "NOTE: See the log file: $logFile for Warnings, etc.  Errors will dump to stdout\n";
PrintRoutines->LogMessage("Building $ARGV[0] MIB dictionary for COLv ...") if $dbg;

## Generate the vendor class name.  This is dependent on the
## passed argv[0] which is one of the following vendors:
##      Bay    (classname = Bay) : Bay Vendor MIBs
##      Nortel (classname = Npp) : Nortel Passport MIBs
##      Cisco  (classname = Cwm) : Cisco WAN Mgr MIBs

my $vclass = "Bay";            # default the vendor class name to Bay
if( $ARGV[0] eq "Nortel" )     # if Nortel, reset the vendor class name
{
    $vclass = "Npp";
}
elsif( $ARGV[0] eq "Cisco" )   # if Cisco, reset the vendor class name
{
    $vclass = "Cwm";
}
## Generate the .cc & .h output file names & open the files for writing
my $CCFile = "MibMeta" . $vclass . "Dict.cc";
my $HdFile = "Mib" . $vclass . ".h";
open OUTFILE1, ">$CCFile" || die "Could not create $CCFile";
open OUTFILE2, ">$HdFile" || die "Could not create $HdFile";

#################################################################################
# ALL the PERL MAGIC Starts here ...
#

## generate the OID Dictionary and SNM MIB list; this is a repeateded
## process till we get a 'definitive' list
my $listmoded = 1;
PrintRoutines->LogMessage("Building the SNM MIB List ....") if $dbg;
while( $listmoded != 0 )
{
    $listmoded = setupSnmMibList( \%MibMetaDict::Dict, \@snmMIBs::snmMIBs,
				  \%oidDict, \%snmMIBList );
    if( $listmoded != 0 )
    {
	PrintRoutines->LogMessage("Added $listmoded attribs into the SNM MIB List. Rebuilding ...") if $dbg;
    }
}
PrintRoutines->LogMessage("SNM MIB List built. #of attributes = ",
			  scalar( keys(%snmMIBList)) ) if $dbg;

## generate the Name, Row & Column dictionaries
setupQuickPointerTables( \%MibMetaDict::Dict, \%snmMIBList,
			 \%oidDict, \%nameDict, \%rowDict, \%colDict );

## generate the parent OID - relative OID relationships
setupTableAttributeRelations( \%colDict, \%rowDict, \%nameDict, \%oidDict );

## populate the enumDict, enumRowDict, enumTypeDict
generateEnumDicts( \%enumDict, \%enumRowDict, \%enumTypeDict,
		   \@arrayEnumStrMap,
		   \%rowDict, \%colDict, \%MibMetaDict::Dict, \%snmMIBList );

PrintRoutines->LogMessage("Extracted ENUM Defns and Vals:") if $dbg;
for $idx ( 0 .. $#arrayEnumStrMap )
{
    PrintRoutines->LogMessage( "[$idx]::[0:txt] = ", $arrayEnumStrMap[$idx][0],
			       ", [1:val] = ", $arrayEnumStrMap[$idx][1]) if $dbg;
}

## Initialize the enum type & class dictionary names
my $mmdClassName = "MibMeta" . $vclass;
my $enumHdrsName = "Mib" . $vclass;
my $mmdCfileHdr  = $mmdClassName . ".h";

## Write the std headers into the output files
codeGen_CopyRight();

## Write the #include directive for the header file into the generated
## C++ file
print OUTFILE1 <<EOT;
#include \<$mmdCfileHdr\>

EOT

# Write the MIB info into the output data files ...
## generate the MIB info data struct in the .cc file
codeGen_MibIdToInfoMap( $mmdClassName,
			\%enumDict, \%enumRowDict, \%enumTypeDict, \%nameDict );

## generate the MIB Table info data struct in the .cc file
codeGen_MibTableIdToInfoMap( $mmdClassName,
			     \%enumDict, \%enumRowDict, \%nameDict );

## generate the Enum types and their values
codeGen_MibTypeIdEnumMap( $mmdClassName, \%enumTypeDict, \@arrayEnumStrMap );

## generate the enum listing for the MIBs in the .h file
codeGen_MibEnums( $enumHdrsName, $vclass, \%enumDict );

# Close the .cc and .h file handles
close(OUTFILE1);
close(OUTFILE2);

PrintRoutines->LogMessage("Unique types found in this vendor MIB :") if $dbg;
foreach my $k ( sort keys(%uniqueTypes) )
{
     PrintRoutines->LogMessage($k) if $dbg;
}
print "Extraction of C++ Dictionaries complete.\n";
exit;
#-- THE END of the PERL magic!
#################################################################################

## Sub Routines follow ....
# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This sroutine generates the MIB Info array (MibIdToInfoMap).  Each array
# entry is a data struct of the form:
#
#    { (r)OID, parent, syntax, access, attrib }
#
# where the attributes represent:
#
#    (r)OID : If the MIB attribute is a Entry MIB (parent), the complete
#           : OID string; for leaf nodes, the relative OID from the parent
#    parent : For leaf nodes, this is the array index of the parent in this
#           : array; for parent nodes, the array index for the MIB table
#           : info (MibTableIdToInfoMap) for info on the parent is stored
#    syntax : For all MIB types except enums, this contains the MIB type;
#           : for enums, this value is = (100 + idx) where 'idx' is the
#           : index into the MibTypeIdEnumMap array for the enum.
#           : the allowed type are declared in the typeStrMap hash table
#    access : Indicates if the MIB attribute has read-only, read-write or
#           : no-access (parent nodes)
#    attrib : Name of the MIB attribute
#
# for e.g.,
#    { "1.3.6.1.2.1.2.2.1", 2, ENTRY, NO_ACCESS, "ifEntry" }, /* 22 */
#    { "13", 22, UNSIGNED32, READ_ONLY, "ifInDiscards" }, /* 23 */
#    { "1", 27, 120, READ_WRITE, "mscFrAtmDlciSiwfSdRowStatus" }, /* 26 */
#
sub codeGen_MibIdToInfoMap
{
    PrintRoutines->LogMessage("=> sub codeGen_MibIdToInfoMap") if $dbg;

    # Extract the passed args in the following order:
    #    mn  -> vendor class name
    #    ed  -> enumDict
    #    erd -> enumRowDict
    #    eed -> enumTypeDict
    #    nd  -> nameDict
    my ($mn, $ed, $erd, $eed, $nd) = @_;

    print OUTFILE1 <<EOT;
//
// Each MibIdToInfoMap array entry is a data struct of the form:
//
//    { (r)OID, parent, syntax, access, attrib }
//
// where the attributes represent:
//
//    (r)OID : If the MIB attribute is a Entry MIB (parent), the complete
//           : OID string; for leaf nodes, the relative OID from the parent
//    parent : For leaf nodes, this is the array index of the parent in this
//           : array; for parent nodes, the array index for the MIB table
//           : info (MibTableIdToInfoMap) for info on the parent is stored
//    syntax : For all MIB types except enums, this contains the MIB type;
//           : for enums, this value is = (100 + idx) where 'idx' is the
//           : index into the MibTypeIdEnumMap array for the enum.
//           : the allowed type are declared in the typeStrMap hash table
//    access : Indicates if the MIB attribute has read-only, read-write or
//           : no-access (parent nodes)
//    attrib : Name of the MIB attribute
//
// for e.g.,
//    { "1.3.6.1.2.1.2.2.1", 2, ENTRY, NO_ACCESS, "ifEntry" }, /* 22 */
//    { "13", 22, UNSIGNED32, READ_ONLY, "ifInDiscards" }, /* 23 */
//    { "1", 27, 120, READ_WRITE, "mscFrAtmDlciSiwfSdRowStatus" }, /* 26 */
//
MibMeta::MibIdToInfoMap ${\$mn}::iim[] = 
{
EOT

    # Use a counter to count down to last file write.  On the last write
    # avoid putting a ',' at the end of the last field.
    my $idx = scalar( keys(%$ed) );
    my $sep = ",";
    PrintRoutines->LogMessage("   Enum Dictionary size = $idx") if $dbg;

    foreach my $n ( sort keys %$ed )
    {
	PrintRoutines->LogMessage("   MibIdInfoMap[", $ed->{$n}, "] obj: ",
				  $nd->{$n}->{"name"}) if $dbg;

	# Extract the OID, for leaf objects, use the relative OID, for
	# parent (ENTRY), use the full OID
	my $oid = $nd->{$n}->{"relOid"};
	## If a relative OID is not defined, it must be an parent node
	if( !defined($oid) )
	{
	    $oid = $nd->{$n}->{"oid"};
	}

	# Generate the MIB entry's Syntax values for SNM (ENUMERATION,
	# INTEGER32, IPADDRESS, ...).  If the actual syntax is defined,
	# use that, else use the base type
	my ($syntax, $actualSyntax);
	my $parentId = $nd->{$n}->{"parent"};
	if( !defined($parentId) )
	{
	    $parentId = $erd->{$n};
	    $syntax = "entry";
	    $actualSyntax = "entry";
#	    print "Warning! Parent id not found for " . $nd->{$n}->{"name"} . "\n";
	    PrintRoutines->LogMessage("   Warning! Parent id not found for ",
				      $nd->{$n}->{"name"}) if $dbg;
	}
	else
	{
	    $parentId = $ed->{$parentId};
	    ($syntax, $actualSyntax) = getSyntax( $nd->{$n},
						  \%MibMetaDict::Dict );
	}
	PrintRoutines->LogMessage("   mapping syntax: ", $actualSyntax) if $dbg;

	if( $actualSyntax ne "" )
	{
	    my $lsyntax = "";
	    $lsyntax = toStringType($actualSyntax,0);
	    if( $lsyntax eq "unknown" )
	    {
		$syntax = toStringType($syntax,1);
	    }
	    else
	    {
		$uniqueTypes{$actualSyntax} = 1;
		$syntax = $lsyntax;
	    }
	}
	else
	{
	    $syntax = toStringType($syntax,1);
	}

	# For Enumerations, we reset it to its Enum dictionary index
	if( $syntax eq "ENUMERATION" )
	{
	    $syntax = $SYNTAX_OFFSET + $eed->{$n}->[0];
	}
	
	# Generate the MIB entry's Access permissions for SNM ( READ_ONLY,
	# READ_WRITE, ... )
	my $access = $nd->{$n}->{"access"};
	if( !defined($access) )
	{
	    $access = "noaccess";
	}
	$access = toStringAccess($access);

	# If this is the last write, set the $sep variable to ""
	$idx--;
	if( $idx == 0 ) { $sep = ""; }

	print OUTFILE1 <<EOT;
    { "$oid", $parentId, $syntax, $access, \"$n\" }$sep /* $ed->{$n} */
EOT
    }

print OUTFILE1 <<EOT;
};

EOT
    PrintRoutines->LogMessage("<= sub codeGen_MibIdToInfoMap") if $dbg;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This sroutine generates the MIB Table array (MibTableIdToInfoMap).  An
# entry exists in the array for each parent node. Each array entry is a
# data struct of the form:
#
#    { indices, attrlist }
#
# where the attributes represent:
#
#     indices : List (i.e., n,m,...) of indexes for this MIB table; these
#             : are attributes that form the index for this table
#    attrlist : Range (i.e., n-m) of leaf nodes that exist under this
#             : parent
#
# for e.g.,
#    { "29", "1-22" }, /* ifEntry 2 */
#
sub codeGen_MibTableIdToInfoMap
{
    PrintRoutines->LogMessage("=> sub codeGen_MibTableIdToInfoMap") if $dbg;

    # Extract the passed args in the following order:
    #    mn  -> vendor class name
    #    ed  -> enumDict
    #    erd -> enumRowDict
    #    nd  -> nameDict
    my ($mn, $ed, $erd, $nd) = @_;

    print OUTFILE1 <<EOT;

// Each MibTableIdToInfoMap array entry is a data struct of the form:
//
//    { indices, attrlist }
//
// where the attributes represent:
//
//     indices : List (i.e., n,m,...) of indexes for this MIB table; these
//             : are attributes that form the index for this table
//    attrlist : Range (i.e., n-m) of leaf nodes that exist under this
//             : parent
//
// for e.g.,
//    { "29", "1-22" }, /* ifEntry 2 */
//
MibMeta::MibTableIdToInfoMap ${\$mn}::itm[] =
{
EOT

    # Use a counter to count down to last file write.  On the last write
    # avoid putting a ',' at the end of the last field.
    my $idx = scalar( keys(%$erd) );
    my $sep = ",";
    PrintRoutines->LogMessage("   enum row dictionary size = $idx") if $dbg;

    foreach my $n ( sort keys %$erd )
    {
	my $indexList   = createIndexList($nd->{$n}, $ed);
	my $attrListStr = createAttrListStr($nd->{$n}->{"attributeList"});

	# if this is the last write, set the $sep variable to ""
	$idx--;
	if( $idx == 0 ) { $sep = ""; }

	print OUTFILE1 <<EOT;
    { \"$indexList\", \"$attrListStr\" }$sep /\* $n $erd->{$n} \*/
EOT
    }

print OUTFILE1 <<EOT;
};

EOT
    PrintRoutines->LogMessage("<= sub codeGen_MibTableIdToInfoMap") if $dbg;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This sroutine generates the MIB Enum mappings array (MibTypeIdEnumMap)
# and MIB Enum values (MibEnumValMap) array. Each array entry of the
# MibTypeIdEnumMap array is a data struct of the form:
#
#    { valArrayIndex, countOfEnum }
#
# where the attributes represent:
#
#    valArrayIndex : Array index in the MibEnumValMap array where the list
#                  : of enum values for this MIB attribute start
#      countOfEnum : Number of enum values for MIB attribute; hence range of
#                  : indices (valArrayIndex - [valArrayIndex+countOfEnum-])
#                  : gives all the possible enum values for a MIB attribute
# for e.g.,
#    { 42, 32}, /* 5, ifType */
#
# Each array entry of the MibEnumValMap array is a data struct of the form:
#
#    { enumName, enumValue }
#
# where the attributes represent:
#
#     enumName : String containing the name of the enum
#    enumValue : Int value of the enum
# for e.g.,
#    { "other", 1 }, /* 42 */
#    { "regular1822", 2 }, /* 43 */
#
sub codeGen_MibTypeIdEnumMap
{
    PrintRoutines->LogMessage("=> sub codeGen_MibTypeIdEnumMap") if $dbg;

    # Extract the passed args in the following order:
    #    mn  -> vendor class name
    #    eed -> enumTypeDict
    #    ae  -> arrayEnumStrMap
    my ($mn, $eed, $ae) = @_;

    print OUTFILE1 <<EOT;
//
// Each MibTypeIdEnumMap array entry is a data struct of the form:
//
//    { valArrayIndex, countOfEnum }
//
// where the attributes represent:
//
//    valArrayIndex : Array index in the MibEnumValMap array where the list
//                  : of enum values for this MIB attribute start
//      countOfEnum : Number of enum values for MIB attribute; hence range of
//                  : indices (valArrayIndex - [valArrayIndex+countOfEnum-])
//                  : gives all the possible enum values for a MIB attribute
// for e.g.,
//    { 42, 32}, /* 5, ifType */
//
MibMeta::MibTypeIdEnumMap ${\$mn}::iem[] =
{
EOT

    # Use a counter to count down to last file write.  On the last write
    # avoid putting a ',' at the end of the last field.
    my $idx = scalar( keys(%$eed) );
    my $sep = ",";
    PrintRoutines->LogMessage("   enum row dictionary size = $idx") if $dbg;

    foreach my $enumType (sort keys %$eed)
    {
	my $enumId = $eed->{$enumType}->[0];
	my $startEnum = $eed->{$enumType}->[1];
	my $countEnums = $eed->{$enumType}->[2];

	# if this is the last write, set the $sep variable to ""
	$idx--;
	if( $idx == 0 ) { $sep = ""; }

	print OUTFILE1 <<EOT;
    { $startEnum, $countEnums}$sep /\* $enumId, $enumType \*/
EOT
    }

print OUTFILE1 <<EOT;
};

EOT

    print OUTFILE1 <<EOT;
//
// Each MibEnumValMap array entry is a data struct of the form:
//
//    { enumName, enumValue }
//
// where the attributes represent:
//
//            enumName : String containing the name of the enum
//           enumValue : Int value of the enum
// for e.g.,
//    { "other", 1 }, /* 42 */
//    { "regular1822", 2 }, /* 43 */
//
MibMeta::MibEnumValMap ${\$mn}::evm[] =
{
EOT
    $idx = scalar(@$ae);
    $sep = ",";
    PrintRoutines->LogMessage("   enum strings array size = $idx") if $dbg;

    my $i = 0;
    foreach my $enumMap (@$ae)
    {
	# if this is the last write, set the $sep variable to ""
	$idx--;
	if( $idx == 0 ) { $sep = ""; }

	print OUTFILE1 <<EOT;
    { \"$enumMap->[0]\", $enumMap->[1] }$sep /\* $i \*/
EOT

    $i++;
    }

print OUTFILE1 <<EOT;
};

EOT
    PrintRoutines->LogMessage("<= sub codeGen_MibTypeIdEnumMap") if $dbg;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This sroutine generates the Mib Enum definitions for the .h file.
#
sub codeGen_MibEnums
{
    PrintRoutines->LogMessage("=> sub codeGen_MibEnums") if $dbg;

    # Extract the passed args in the following order:
    #    cn  -> enumHdrsName : Name for the enum type defined in the .h file
    #    vnd -> vclass : Vendor name
    #    ed  -> enumDict
    my ($cn, $vnd, $ed) = @_;

    print OUTFILE2 <<EOT;
#ifndef _${\$cn}_h
#define _${\$cn}_h

class $cn
{
public:
    typedef enum $vnd 
    {
EOT

    # Use a counter to count down to last file write.  On the last write
    # avoid putting a ',' at the end of the last field.
    my $idx = scalar( keys(%$ed) );
    my $sep = ",";
    PrintRoutines->LogMessage("   enum dictionary size = $idx") if $dbg;

    foreach my $enum (sort keys %$ed)
    {
	# if this is the last write, set the $sep variable to ""
	$idx--;
	if( $idx == 0 ) { $sep = ""; }

	print OUTFILE2 <<EOT;
        $enum$sep  /* $ed->{"$enum"} */
EOT
    }

print OUTFILE2 <<EOT;
    };
};

#endif
EOT
    PrintRoutines->LogMessage("<= sub codeGen_MibEnums") if $dbg;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
sub getSyntax
{
    PrintRoutines->LogMessage("=> sub getSyntax") if $dbg;

    my ($node, $d) = @_;
    my $type;
    my $syntax = $node->{"syntax"};

    if( defined($type = $syntax->{"type"}->{"basetype"}) )
    {
	$uniqueTypes{$type} = 1;
	PrintRoutines->LogMessage("   syntax = $type") if $dbg;
	PrintRoutines->LogMessage("<= sub getSyntax") if $dbg;
	return $type;
    }
    elsif( defined($type = $syntax->{"type"}->{"module"}) )
    {
	my $typeModule = $syntax->{"type"}->{"module"};
	my $typeName = $syntax->{"type"}->{"name"};
	PrintRoutines->LogMessage("   module = $typeModule") if $dbg;

	if( $typeName =~ /DisplayString/i && $typeModule =~ m/rfc1213/i )
	{
	    $uniqueTypes{$typeName} = 1;
	    PrintRoutines->LogMessage("   syntax = DisplayString") if $dbg;
	    PrintRoutines->LogMessage("<= sub getSyntax") if $dbg;
	    return ("DisplayString", "");
	}

	if( $typeModule eq "" )
	{
	    $uniqueTypes{$typeName} = 1;
	    PrintRoutines->LogMessage("   syntax = $typeName, actual = (null) ") if $dbg;
	    PrintRoutines->LogMessage("<= sub getSyntax") if $dbg;
	    return ($typeName, "");
	}

	$type =  $d->{$typeModule}->{"typedefs"}->{$typeName}->{"basetype"};
	$uniqueTypes{$type} = 1;
	PrintRoutines->LogMessage("   syntax = $type, actual = $typeName") if $dbg;
	PrintRoutines->LogMessage("<= sub getSyntax") if $dbg;
	return ($type, $typeName);
    }
    else
    {
#	print "Warning! Syntax for " . $node->{"name"} . " defaulted to ENTRY\n";
	PrintRoutines->LogMessage("   Warning! Empty 'syntax' specification for: ",
				  $node->{"name"}, ". Defaulting to ENTRY");
	return ("entry","entry");
    }

    PrintRoutines->LogMessage("<= sub getSyntax") if $dbg;
    return "";
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This sroutine generates the dictionary for all the MIBs are of an enum
# type (EnumRow dictionary).
#
sub generateEnumDicts
{
    PrintRoutines->LogMessage("=> sub generateEnumDicts") if $dbg;

    # Extract the passed args in the following order:
    #    ed  -> enumDict                 (initialize/write)
    #    erd -> enumRowDict              (initialize/write)
    #    eed -> enumTypeDict             (initialize/write)
    #    arrayEnums -> arrayEnumStrMap   (initialize/write)
    #    rd  -> rowDict                  (read only)
    #    cd  -> colDict                  (read only)
    #    d   -> MibMetaDict::Dict        (read only)
    # snmlst ->  snmMIBList              (read only)
    my ($ed, $erd, $eed, $arrayEnums, $rd, $cd, $d, $snmlst) = @_;

    # extract an array of the Row dictionary keys
    my @mibNames = keys(%$rd);

    # store keys into the EnumRow dictionary
    PrintRoutines->LogMessage("   generating EnumRowDict dictionary ...") if $dbg;
    my $i = 0;
    foreach my $k ( sort(@mibNames) )
    {
	PrintRoutines->LogMessage("   enumRowDict{ $k } = $i") if $dbg;
	$erd->{$k} = $i++;
    }

    # extract the keys of the Column dictionary and store the entire
    # 'mibNames' array into the enum Dictionary
    push @mibNames, keys(%$cd);

    $i = 0;
    PrintRoutines->LogMessage("   generating EnumDict dictionary ...") if $dbg;
    foreach my $k ( sort(@mibNames) )
    {
	PrintRoutines->LogMessage("   enumDict{ $k } = $i") if $dbg;
	$ed->{$k} = $i++;
    }

    # extract the key of the MibMetaDict::Dict dictionary (module names)
    # and process the Enumerations in them.
    my ($startEnum) = (0);
    foreach my $module ( keys (%$d) )
    {
	PrintRoutines->LogMessage("   iter.ng MibMetaDict::Dict MODULE: $module") if $dbg;
	while( my ($typename, $type) = each(%{$d->{$module}->{"typedefs"}}) )
	{
	    if( "Enumeration" eq $type->{"basetype"} )
	    {
		local($rangeS) = $startEnum;
		populateEnumTypeDict( $eed, $module, $typename, $type,
				      $arrayEnums, \$startEnum );
		local($rangeE) = $startEnum - 1;
		PrintRoutines->LogMessage("   mod enum: ", $node->{"name"}, " range $rangeS:$rangeE") if $dbg;
	    }
	}

	# process all the nodes within the current module
	foreach my $node ( @{$d->{$module}->{"nodes"}} )
	{
	    PrintRoutines->LogMessage("   prcc.ng MibMetaDict::Dict Node: ", $node->{"name"}) if $dbg;
	    # update the enum dictionary only if the node is in the SNM miblist
	    # AND the node has an enum 'syntax' defined
	    local $nodename = $node->{"name"};
	    if( (exists($snmlst->{$nodename})) && (defined($node->{"syntax"})) )
	    {
		my $type;
		my $syntax = $node->{"syntax"};
		if( defined($type = $syntax->{"type"}->{"basetype"}) )
		{
		    if( "Enumeration" eq $node->{"syntax"}->{"type"}->{"basetype"} )
		    {
			local($rangeS) = $startEnum;
			populateEnumTypeDict( $eed, $module,
					      $node->{"name"},
					      $node->{"syntax"}->{"type"},
					      $arrayEnums, \$startEnum );
			local($rangeE) = $startEnum - 1;
			PrintRoutines->LogMessage("   enum: ", $node->{"name"}, " range $rangeS:$rangeE") if $dbg;
		    }
		}
		elsif( defined($syntax->{"type"}->{"module"}) )
		{
		    my $typeModule = $syntax->{"type"}->{"module"};
		    my $typeName = $syntax->{"type"}->{"name"};

		    if( defined($typeModule) && $typeModule ne "" )
		    {
			$type =  $d->{$typeModule}->{"typedefs"}->{$typeName}->{"basetype"};

			if( "Enumeration" eq $type )
			{
			    local($rangeS) = $startEnum;
			    populateEnumTypeDict( $eed, $module, $node->{"name"},
						  $d->{$typeModule}->{"typedefs"}->{$typeName},
						  $arrayEnums, \$startEnum );
			    local($rangeE) = $startEnum - 1;
			    PrintRoutines->LogMessage("   typdef'd enum: ", $node->{"name"}, " range $rangeS:$rangeE") if $dbg;
			}
		    }
		}
	    }
	}
    }

    $i = 0;
    foreach my $enumType ( sort keys %$eed )
    {
	PrintRoutines->LogMessage("   ID for $enumType = $i") if $dbg;
	$eed->{$enumType}->[0] = $i++;
    }

    PrintRoutines->LogMessage("<= sub generateEnumDicts") if $dbg;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
sub populateEnumTypeDict
{
    PrintRoutines->LogMessage("=> sub populateEnumTypeDict") if $dbg;

    # Extract the passed args in the following order:
    #    eed        -> enumDict
    #    module     -> name of the MIB module
    #    typename   -> enum name
    #    type       -> enum entries
    #    arrayEnums -> arrayEnumStrMap
    #    startEnum  -> global index to count the enum types

    my ($eed, $module, $typename, $type, $arrayEnums, $startEnum) = @_;
    PrintRoutines->LogMessage("   populating enum: $typename") if $dbg;

    if( defined($eed->{$typename}) )
    {
	print "ERROR! ", $module, ":", $typename, " is duplicated \n ";
	exit 1;
	#next;
    }

    $eed->{$typename}->[0] = 0; # enum-id fill in later
    $eed->{$typename}->[1] = $$startEnum;

    my %enumValToStr;
    while( my($en, $ev) = each(%$type) )
    {
	if( defined($enumValToStr{$ev}) )
	{
	    print "ERROR! Duplicate enum str ", $en, " for value", $ev, "! \n";
	}
	$enumValToStr{$ev} = $en;
    }

    my $enumCount = 0;
    foreach my $ev ( sort {$a <=> $b} keys %enumValToStr )
    {
	next if ($ev !~ /\d+/);
	push @$arrayEnums, [$enumValToStr{$ev}, $ev];
	$$startEnum++;
	$enumCount++;
    }
    %enumValToStr = (); # release the mem, does this work ?

    $eed->{$typename}->[2] = $enumCount;

    PrintRoutines->LogMessage("<= sub populateEnumTypeDict") if $dbg;
}


# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This sroutine builds the OID dictionary and the list of MIBs SNM uses.
sub setupSnmMibList
{
    PrintRoutines->LogMessage("=> sub setupSnmMibList") if $dbg;

    # Extract the passed args in the following order:
    #      d -> MibMetaDict::Dict  (read only)
    #   mibs -> snmMIBs::snmMIBs   (read only)
    #     od -> oidDict            (initialize)
    # snmlst -> snmMIBList         (initialize)
    my ($d, $mibs, $od, $snmlst) = @_;

    # return value to indicate if the list was modified
    my $retval = 0;

    # Iterate over the generated SMI dump MIB dictionary and extract all
    # the OIDs into the OID Dictionary (%od)
    PrintRoutines->LogMessage("   Populating OID dictionary ...") if $dbg;
    while( my ($moduleName, $module) = each(%$d) )
    {
	foreach $node ( @{$module->{"nodes"}} )
	{
	    # by default we add all the entries into the OID dictionary
	    $od->{$node->{"oid"}} = $node;
	}
    }

    # Create a a local hash 'MIB list' containing all the MIBs that SNMs
    # uses for a particular vendor so that 'existence' checking is mucho faster
    # (done only the first time)
    if( scalar( keys(%$snmlst) ) == 0 )
    {
	  PrintRoutines->LogMessage("   Generating default MIB list ...") if $dbg;
	  foreach $idx (@$mibs) 
	  {
	      $snmlst->{$idx} = $idx;
	      $retval++;
	  }
    }

    # Insert any dependent parent nodes and table-index nodes into the MIB list;
    # this is tricky ... for column nodes: the parent node and any index MIBs are
    # in the 'MIB list'; for row nodes: the index MIBs are in the 'MIB list'
    while( my ($moduleName, $module) = each(%$d) )
    {
	PrintRoutines->LogMessage("   prcss.ng module: $moduleName") if $dbg;
	foreach $node ( @{$module->{"nodes"}} )
	{
	    local $nodename = $node->{"name"};
	    PrintRoutines->LogMessage("   Analyzing node: $nodename") if $dbg;

	    if( exists($snmlst->{$nodename}) )
	    {
		PrintRoutines->LogMessage("   Expanding MIB list to support $nodename") if $dbg;
		if(($node->{"type"} eq "column") || ($node->{"type"} eq "scalar"))
		{
		    my ($parentOid, $myRelOid) = getParentOid($node->{"oid"});
		    if( defined($od->{$parentOid}) )
		    {
			local $pnodename = $od->{$parentOid}->{"name"};
			if( ! exists($snmlst->{$pnodename}) )
			{
			    # Added the parent node into the list and increment
			    # the 'list modified' flag so that another iteration
			    # will be triggered
			    PrintRoutines->LogMessage("   Index: $pnodename added to MIB list") if $dbg;
			    $snmlst->{$pnodename} = $pnodename;
			    $retval++;
			    local $pnode = $od->{$parentOid};
			    if( exists $pnode->{"index"} )
			    {
				foreach $idxmib ( @{$pnode->{"index"}} )
				{
				    $idxmibname = $idxmib->{"name"};
				    if( ! exists($snmlst->{$idxmibname}) )
				    {
					# add the index and increment the 'list
					# modified' flag
					PrintRoutines->LogMessage("   Index: $idxmibname added to MIB list") if $dbg;
					$snmlst->{$idxmibname} = $idxmibname;
					$retval++;
				    }
				}
			    }
			}
		    }
		    else
		    {
			print "ERROR! $nodename OID not found in OID Dictionary!!\n";
			exit 1;
		    }
		}
		elsif(($node->{"type"} eq "row") || ($node->{"type"} eq "node"))
		{
		    if( exists $node->{"index"} )
		    {
			foreach $idxmib ( @{$node->{"index"}} )
			{
			    $idxmibname = $idxmib->{"name"};
			    if( ! exists($snmlst->{$idxmibname}) )
			    {
				# add the index and incr the 'list modified' flag
				PrintRoutines->LogMessage("   Index: $idxmibname added to MIB list") if $dbg;
				$snmlst->{$idxmibname} = $idxmibname;
				$retval++;
			    }
			}
		    }
		}
		else
		{
		    print "ERROR! $nodename is not of a regular MIB type(row/column/node/scalar)\n";
		    exit 1;
		}
	    }
	    else
	    {
		# Check for the "ATTRIB list bug": if an 'ENTRY'(row) MIB is
		# defined in the SNM list, all the attribute MIBs (columns)
		# under it, _should_ be included for dictionary generation;
		# else, an incorrect attribute list is generated.
		if(($node->{"type"} eq "column") || ($node->{"type"} eq "scalar"))
		{
		    my ($parentOid, $myRelOid) = getParentOid($node->{"oid"});
		    if( defined($od->{$parentOid}) )
		    {
			local $pnodename = $od->{$parentOid}->{"name"};
			if( exists($snmlst->{$pnodename}) )
			{
			    # add the index and incr the 'list modified' flag
			    PrintRoutines->LogMessage("   $nodename added to fix the 'attriblist' bug ...") if $dbg;
			    $snmlst->{$nodename} = $nodename;
			    $retval++;
			}
			else
			{
			    PrintRoutines->LogMessage("   $nodename ignored (parent also not in the SNM list)") if $dbg;
			}
		    }
		    else
		    {
			print "ERROR! $nodename OID not found in OID Dictionary!!\n";
			exit 1;
		    }
		}
		else
		{
#		    print "Warning! $nodename ignored (Not in SNM MIB list)\n";
		    PrintRoutines->LogMessage("   Warning! $nodename ignored (Not in SNMMib list)") if $dbg;
		}
	    }
	}
    }
    # print the SNM (all inclusive) MIB list
    PrintRoutines->LogMessage("   All Inclusive list of SNM MIBs:") if $dbg;
    foreach $k ( sort keys(%$snmlst) )
    {
	PrintRoutines->LogMessage("   MIB: $k") if $dbg;
    }

    PrintRoutines->LogMessage("<= sub setupSnmMibList") if $dbg;
    return $retval;
}


# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This routine builds/inits the oid, name, row & column dictionaries from
# the 'Dict' hash table in the MibMetaDict Perl module generated by the
# SMI dump program.
#
sub setupQuickPointerTables
{
    PrintRoutines->LogMessage("=> sub setupQuickPointerTables") if $dbg;

    # Extract the passed args in the following order:
    #      d -> MibMetaDict::Dict  (read only)
    # snmlst -> snmMIBList         (read only)
    #     od -> oidDict            (write)
    #     nd -> nameDict           (write)
    #     rd -> rowDict            (write)
    #     cd -> colDict            (write)
    my ($d, $snmlst, $od, $nd, $rd, $cd) = @_;

    # Generate the Name, Row & Column dictionaries
    PrintRoutines->LogMessage("   Generating Name, Row & Col dictionaries...") if $dbg;
    while( my ($moduleName, $module) = each(%$d) )
    {
	PrintRoutines->LogMessage("   MIB MODULE: $moduleName") if $dbg;

	foreach $node ( @{$module->{"nodes"}} )
	{
	    local $nodename = $node->{"name"};
	    local $msg = "   Node: " . $nodename . " ...";
	    if( ($node->{"type"} eq "row") || ($node->{"type"} eq "node") )
	    {
		if( defined($nd->{$nodename}) )
		{
		    print "ERROR! ", $nodename, " has been Duplicated!\n";
		}
		elsif( exists($snmlst->{$nodename}) )
		{
		    # if this mib is in the 'needed' array, add it to the
		    # name & row dictionaries
		    $nd->{$nodename} = $node;
		    $rd->{$nodename} = $node;
		    PrintRoutines->LogMessage("$msg -> (Name & ROW) dictionaries") if $dbg;
		    # update the snmlst collection to contain the indexes
		    # used by this row/node (if its already not in there)
		}
		else
		{
		    PrintRoutines->LogMessage("$msg -> Node/ROW ignored!?") if $dbg;
		}
	    }
	    elsif( ($node->{"type"} eq "column") || ($node->{"type"} eq "scalar") )
	    {
		if( defined($nd->{$nodename}) )
		{
		    print "ERROR! ", $nodename, " has been Duplicated!\n";
		}
		elsif( exists($snmlst->{$nodename}) )
		{
		    $nd->{$nodename} = $node;
		    $cd->{$nodename} = $node;
		    PrintRoutines->LogMessage("$msg -> (Name & COL) dictionaries") if $dbg;
		}
		else
		{
		    PrintRoutines->LogMessage("$msg -> Scalar/COL ignored!?") if $dbg;
		}
	    }
	    else
	    {
		PrintRoutines->LogMessage("$msg -> Ignored?") if $dbg;
	    }
	}   # END foreach $node
    }

    PrintRoutines->LogMessage("<= sub setupQuickPointerTables") if $dbg;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This subroutine takes each entry in the Column dictionary and determines
# the parent MIB attribute.  It then adds 2 new column attributes- parent
# name and relative OID, to the entry in the Column dictionary.  Also, if
# the parent node does not exist in the Row & Name dictionaries, it updates
# the dictionaries with the parent node's information.
#
sub setupTableAttributeRelations
{
    PrintRoutines->LogMessage("=> sub setupTableAttributeRelations") if $dbg;

    # Extract the passed args in the following order:
    #    cd -> colDict  (update)
    #    rd -> rowDict  (update)
    #    nd -> nameDict (update)
    #    od -> oidDict  (read)
    my ($cd, $rd, $nd, $od) = @_;

    # iterate over the Column dictionary and setup the parent-to-child
    # OID relationships
    PrintRoutines->LogMessage("   mapping child-to-parent in the COLUMN dictionary ...") if $dbg;
    while( my($n, $v) = each(%$cd) )
    {
	PrintRoutines->LogMessage("   prcss.ng column: $n") if $dbg;
	# determine the entry's parent
	my ($parentOid, $myRelOid) = getParentOid($v->{"oid"});
	local $parname = $od->{$parentOid}->{"name"};
	
	if( defined($od->{$parentOid}) )
	{
	    # check if the parent node exists in the Row dictionary; if
	    # absent, add it
	    if( ( !exists($rd->{$parname}) ) and exists($snmlst->{$parname}) )
	    {
		PrintRoutines->LogMessage( "   ", $parname,
					   " +> Name & ROW dictionaries") if $dbg;
		$rd->{$parname} = $od->{$parentOid};
		$nd->{$parname} = $od->{$parentOid};
	    }

	    # create the hash entries for the parent name & rel OID
	    $v->{"parent"} = $parname;
	    $v->{"relOid"} = $myRelOid;
	    # add the relative OID to the parent node's attribute list
	    push @{$od->{$parentOid}->{"attributeList"}} , $myRelOid;
	    PrintRoutines->LogMessage( "   ", $n, "'s parent: ", $parname,
				       ", rOID: ", $myRelOid ) if $dbg;
	}
	else
	{
	    # parent not found!!
	    PrintRoutines->LogMessage( "   ", $n,
				       "'s parent not found in the OID Dictionary!");
	}
    }

    PrintRoutines->LogMessage("<= sub setupTableAttributeRelations") if $dbg;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
sub getParentOid
{
#    PrintRoutines->LogMessage("=> sub getParentOid") if $dbg;

    my ($oid) = shift;
    if( $oid =~ m/(.*)\.(\d+)$/ )
    {
#	PrintRoutines->LogMessage("   Par OID = $1, rOID = $2") if $dbg;
#	PrintRoutines->LogMessage("<= sub getParentOid") if $dbg;
	return ($1, $2);
    }

#    PrintRoutines->LogMessage("<= sub getParentOid") if $dbg;
    return ();
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
sub createAttrListStr
{
    PrintRoutines->LogMessage("=> sub createAttrListStr") if $dbg;

    my $al = shift;
    @$al = sort {$a <=> $b} (@$al);
#    PrintRoutines->PrintArray( @$al ) if $dbg;

    my ($prev, $start, $more, $rangeSize) = (-1, -1, -1, 0);
    my $outStr;
    foreach my $v (@$al)
    {
	if( -1 == $prev )
	{
	    $start = $v;
	    $prev = $v;
	}
	else
	{
	    if( $v != $prev+1 )
	    {
		my $str;
		if( $rangeSize )
		{
		    $str = ($start . "-" . $prev);
#		    print "range ", $rangeSize, " : ", $str, "\n";
		}
		else
		{
		    $str = $prev;
#		    print "range ", $rangeSize, " : ", $str,"\n";
		}
		$outStr = $outStr . "$str,";
		$prev = $v;
		$start = $v;
		$rangeSize = 0;
	    }
	    else
	    {
#		print "rangeSize++\n";
		$rangeSize++;
		$prev = $v;
	    }
	}
    }

    if( $rangeSize )
    {
	$outStr = $outStr . $start . "-" . $al->[-1];
    }
    else
    {
	$outStr = $outStr . $start;
    }

    $outStr =~ s/,$//;

    PrintRoutines->LogMessage("   indices: $outStr") if $dbg;
    PrintRoutines->LogMessage("<= sub createAttrListStr") if $dbg;
    return $outStr;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
sub createIndexList
{
    PrintRoutines->LogMessage("=> sub createIndexList") if $dbg;

    my ($node, $ed) = @_;
    my $outStr;
    foreach my $index (@{$node->{"index"}})
    {
	$outStr = $outStr . $ed->{$index->{"name"}} . ",";
    }
    $outStr =~ s/,$//;

    PrintRoutines->LogMessage("<= sub createIndexList") if $dbg;
    return $outStr;
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
sub toStringAccess
{
    PrintRoutines->LogMessage("=> sub toStringAccess") if $dbg;

    my $str = shift;
    if(defined($accessStrMap{$str}))
    {
	PrintRoutines->LogMessage("<= sub toStringAccess") if $dbg;
	return $accessStrMap{$str};
    }

    print "ERROR! Unknown type [$str] not being handled\n";
    exit 1;
    #return "<$str>";
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
sub toStringType
{
    PrintRoutines->LogMessage("=> sub toStringType") if $dbg;

    my $str = shift;
    my $chkexit = shift;

    if( defined($typeStrMap{$str}) )
    {
	PrintRoutines->LogMessage("<= sub toStringType") if $dbg;
	return $typeStrMap{$str};
    }

    if( $chkexit == 1 )
    {
	print "ERROR! Unknown type [$str] not being handled\n";
	exit 1;
    }

    return "unknown";
}

# ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#
sub codeGen_CopyRight
{
print OUTFILE1 <<EOT;
//
//    !!! THIS FILE IS AUTOMATICALLY GENERATED !!! DO NOT EDIT !!!
//
//         Refer to (COLV)/mibmeta/HOWTO-MibMetaDictionary for
//         auto-generation instructions
//
EOT

print OUTFILE2 <<EOT;
//
//    !!! THIS FILE IS AUTOMATICALLY GENERATED !!! DO NOT EDIT !!!
//
//         Refer to (COLV)/mibmeta/HOWTO-MibMetaDictionary for
//         auto-generation instructions
//
EOT

}

##
## <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<  CHANGE LOG  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
##
## Mar/25/06 : Adapted from convert2.pl which was a Nortel specific
##             script generated by Asit/Paresh. Made the script generic
##             so that it can be used for Bay and Nortel too.
## Jun/02/06 : Added debug tracing.  Execute the script with a
##             '-dbg' option.
## Jun/04/06 : Added logging.  Execute with a -log=<filename>
## Jun/10/06 : Modified to use the snmMIBs.pm module which lists the
##             MIBs used by SNM.  There needs to be a separate module
##             created for each vendor.
## Jun/11/06 : [jf] Modified all the file write sub routines to not put a comma
##             at the end of the last element.  This prevents a irritating
##             warning from showing up during compiling every COLv file!
## Jun/14/06 : [jf] Added Cisco support
## Aug/04/06 : [jf] Added support for the following MiB syntices- { IpAddress,
##             Counter, Gauge, NetworkAddress, PhysAddress, TimeTicks }
##             to the 'typeStrMap' array.
##
