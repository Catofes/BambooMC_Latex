#include "detector/SampleWorldBox.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>

// anonymous namespace to register the SampleWorldBox

namespace {

  BambooDetectorPart * createSampleWorldBox (const G4String & name)
  {
    return new SampleWorldBox(name);
  }

  const std::string SampleWorldBoxName("SampleWorldBox");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(SampleWorldBoxName, createSampleWorldBox);
}

SampleWorldBox::SampleWorldBox (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);
  _halfX = dp.getParameterAsDouble("half_x");
  _halfY = dp.getParameterAsDouble("half_y");
  _halfZ = dp.getParameterAsDouble("half_z");
}

G4bool SampleWorldBox::construct ()
{
  G4Material * air = G4Material::GetMaterial("G4_AIR");
  G4Box * worldBox = new G4Box("WorldBox", _halfX, _halfY, _halfZ);
  _partLogicalVolume = new G4LogicalVolume(worldBox, air, "WorldLog", 0, 0, 0);
  _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "World", 0, false, 0);
  _partContainerLogicalVolume = _partLogicalVolume;
  G4VisAttributes * worldVis = new G4VisAttributes();
  worldVis->SetVisibility(false);
  _partLogicalVolume->SetVisAttributes(worldVis);
  return true;
}

