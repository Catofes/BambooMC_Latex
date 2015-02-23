#include "detector/SampleBoxDetector.hh"
#include "detector/BambooDetectorFactory.hh"
#include "analysis/PandaXLXeSensitiveDetector.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>

// anonymous namespace to register the SampleBoxDetector

namespace {

  BambooDetectorPart * createSampleBoxDetector ()
  {
    return new SampleBoxDetector("SampleBoxDetector");
  }

  const std::string SampleBoxDetectorName("SampleBoxDetector");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(SampleBoxDetectorName, createSampleBoxDetector);
}

SampleBoxDetector::SampleBoxDetector (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters("SampleBoxDetector");
  _halfX = dp.getParameterAsDouble("half_x");
  _halfY = dp.getParameterAsDouble("half_y");
  _halfZ = dp.getParameterAsDouble("half_z");
}

G4bool SampleBoxDetector::construct ()
{
  G4NistManager* pNistManager = G4NistManager::Instance();
  pNistManager->FindOrBuildMaterial("G4_lXe");
  G4Material * xenon = G4Material::GetMaterial("G4_lXe");
  G4Box * sampleDetectorBox = new G4Box("SampleDetectorBox", _halfX, _halfY, _halfZ);
  _partLogicalVolume = new G4LogicalVolume(sampleDetectorBox, xenon, "SampleDetectorLog", 0, 0, 0);
  _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "SampleDetector", _parentPart->getContainerLogicalVolume(), false, 0);
  _partContainerLogicalVolume = _partLogicalVolume;
  PandaXLXeSensitiveDetector * lXeSD = new PandaXLXeSensitiveDetector("LXeSD");
  G4SDManager * sdManager = G4SDManager::GetSDMpointer();
  sdManager->AddNewDetector(lXeSD);
  _partLogicalVolume->SetSensitiveDetector(lXeSD);
  return true;
}

