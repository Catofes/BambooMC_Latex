#include "detector/SampleCylinderDetector.hh"
#include "detector/BambooDetectorFactory.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4Tubs.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>

#include <G4VisAttributes.hh>

#include <math.h>

// anonymous namespace to register the SampleCylinderDetector

namespace {

  BambooDetectorPart * createSampleCylinderDetector (const G4String & name)
  {
    return new SampleCylinderDetector(name);
  }

  const std::string SampleCylinderDetectorName("SampleCylinderDetector");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(SampleCylinderDetectorName, createSampleCylinderDetector);
}

SampleCylinderDetector::SampleCylinderDetector (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);
  _radius = dp.getParameterAsDouble("radius") * mm;
  _height = dp.getParameterAsDouble("height") * mm;
  _shiftX = dp.getParameterAsDouble("shiftX") * mm;
  _shiftY = dp.getParameterAsDouble("shiftY") * mm;
  _shiftZ = dp.getParameterAsDouble("shiftZ") * mm;
  G4cout << "radius: " <<  _radius/mm << "mm." << G4endl;
}

G4bool SampleCylinderDetector::construct ()
{
  G4Material * liquidXenon = G4Material::GetMaterial("G4_lXe");
  G4Tubs * sampleDetectorTub = new G4Tubs("SampleDetectorTub", 0, _radius, _height/2., 0., 2.* M_PI);
  _partLogicalVolume = new G4LogicalVolume(sampleDetectorTub, liquidXenon
                                           , "SampleDetectorLog", 0, 0, 0);
  _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ), _partLogicalVolume, "SampleDetector", _parentPart->getContainerLogicalVolume(), false, 0);
  _partContainerLogicalVolume = _partLogicalVolume;
  G4VisAttributes * sampleDetectorVisAtt = new G4VisAttributes(G4Colour(0.,0.6, 0.1));
  _partLogicalVolume->SetVisAttributes(sampleDetectorVisAtt);
  PandaXSensitiveDetector * cylinderSD = new PandaXSensitiveDetector("SampleCylinderSD");
  G4SDManager * sdManager = G4SDManager::GetSDMpointer();
  sdManager->AddNewDetector(cylinderSD);
  _partLogicalVolume->SetSensitiveDetector(cylinderSD);
  G4cout << "Sensitive Mass: " << _partLogicalVolume->GetMass()/kg << " kg." << G4endl;
  return true;
}

