#!/usr/bin/perl -w

use strict;

my $type = shift or die "Usage: $0 (detector|material|physics|analysis) className\n";
my $class_name = shift or die "Usage: $0 (detector|material|physics|analysis) className\n";

if ($type ne "detector" && $type ne "material" && $type ne "physics" && $type ne "analysis") {
  die "Type must be one of the four: detector, material, physics, analysis.\n";
}

my $uc_name = $class_name;
$uc_name =~ tr/a-z/A-Z/;

my %header_directory = ( "detector" => "detector",
			 "material" => "detector",
			 "physics" => "physics",
			 "analysis" => "analysis"
	      );

my %parent_classes = ( "detector" => "BambooDetectorPart",
		       "material" => "BambooMaterial",
		       "physics" => "BambooPhysics",
		       "analysis" => "BambooAnalysis"
		     );

my %factory = ( "detector" => "BambooDetectorFactory",
		"material" => "BambooMaterialFactory",
		"physics" => "BambooPhysicsFactory",
		"analysis" => "BambooAnalysisFactory"
	      );


my %register_type = ( "detector" => "registerDetectorPart",
		      "material" => "registerMaterial",
		      "physics" => "registerPhysics",
		      "analysis" => "registerAnalysis"
		    );

my %function_declaration = ( "detector" => "  virtual G4bool construct();",
			     "material" => "  virtual void defineMaterials();",
			     "physics" => "",
			     "analysis" => ""
		);

my $sample_header = <<"EOF";
#ifndef ${uc_name}_H
#define ${uc_name}_H

#include "$header_directory{$type}/$parent_classes{$type}.hh"

class $class_name : public $parent_classes{$type}
{

public:

  $class_name(const G4String &name);

$function_declaration{$type}

private:

};


#endif // ${uc_name}_H

EOF

# create header file.
open my $fh_header, ">", "$class_name.hh" or die "Can't open file $class_name.hh\n";
print $fh_header $sample_header;

close $fh_header;

my $sample_source = <<"EOF";
#include "$header_directory{$type}/$class_name.hh"
#include "$header_directory{$type}/$factory{$type}.hh"
#include "BambooGlobalVariables.hh"

// anonymous namespace to register the $class_name

namespace {

  $parent_classes{$type} * create${class_name} ()
  {
    return new $class_name("$class_name");
  }

  const std::string ${class_name}Name("$class_name");

  const bool registered = $factory{$type}::Instance()->$register_type{$type}(${class_name}Name, create${class_name});
}

$class_name::$class_name (const G4String & name)
  : $parent_classes{$type}(name)
{
  BambooGlobalVariables *bgv = BambooGlobalVariables::Instance();
  G4cout << "$class_name found..." << G4endl;
}


@{[ function_definition($type) ]}


EOF

# create header file.
open my $fh_source, ">", "$class_name.cc" or die "Can't open file $class_name.cc\n";
print $fh_source $sample_source;
close $fh_source;

print "please copy the files to the right directories.\n";

sub function_definition {
  my $type = shift;
  my $res = "";
  if ($type eq "detector") {
    $res = "G4bool ${class_name}::construct\(\)\n{\n  // add code here ...\n  return true;\n}\n";
  } elsif ($type eq "material" ) {
    $res = "void ${class_name}::defineMaterials\(\)\n{\n  // add code here ...\n  ;\n}\n";
  }
  $res;
}
