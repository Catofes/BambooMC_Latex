#!/usr/bin/perl -w

use strict;

my $part_name = shift or die "Usage: $0 PartName\n";

my $uc_name = $part_name;
$uc_name =~ tr/a-z/A-Z/;

my $sample_header = <<"EOF";
#ifndef ${uc_name}_H
#define ${uc_name}_H

#include "detector/BambooDetectorPart.hh"

class $part_name : public BambooDetectorPart
{

public:

  $part_name(const G4String &name);

  virtual G4bool construct();

private:

};


#endif // ${uc_name}_H

EOF

# create header file.
open my $fh_header, ">", "$part_name.hh" or die "Can't open file $part_name.hh\n";
print $fh_header $sample_header;

close $fh_header;

my $sample_source = <<"EOF";
#include "detector/$part_name.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

// anonymous namespace to register the $part_name

namespace {

  $part_name * create${part_name} ()
  {
    return new $part_name("$part_name");
  }

  const std::string ${part_name}Name("$part_name");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(${part_name}Name, create${part_name});
}

$part_name::$part_name (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters("$part_name");
  G4cout << "$part_name found..." << G4endl;
}

G4bool ${part_name}::construct ()
{
  // add construction code here ...
  return true;
}

EOF

# create header file.
open my $fh_source, ">", "$part_name.cc" or die "Can't open file $part_name.cc\n";
print $fh_source $sample_source;
close $fh_source;

print "please copy the files to the right directories.\n";
