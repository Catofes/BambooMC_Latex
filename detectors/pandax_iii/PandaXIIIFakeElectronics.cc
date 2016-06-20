#include "detector/PandaXIIIFakeElectronics.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"

#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4Tubs.hh>

// anonymous namespace to register the PandaXIIIFakeElectronics

namespace {

  BambooDetectorPart * createPandaXIIIFakeElectronics (const G4String & name)
  {
    return new PandaXIIIFakeElectronics(name);
  }

  const std::string PandaXIIIFakeElectronicsName("PandaXIIIFakeElectronics");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXIIIFakeElectronicsName, createPandaXIIIFakeElectronics);
}

PandaXIIIFakeElectronics::PandaXIIIFakeElectronics (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);

  _radius = BambooUtils::evaluate(dp.getParameterAsString("radius"));
  _distance = BambooUtils::evaluate(dp.getParameterAsString("distance"));

  if (_radius <= 0) {
    _radius = 750 * mm;
  }
  if (_distance <= 0) {
    _distance = 1.15*m + 0.01*m;
  }
  G4cout << "PandaXIIIFakeElectronics..." << G4endl;
}


G4bool PandaXIIIFakeElectronics::construct()
{
  G4Material * copper = G4Material::GetMaterial("G4_Cu");
  G4VSolid * electronicsTub = new G4Tubs("ElectronicsTub", 0, _radius, 1*cm, 0, 2*pi);
  G4LogicalVolume * electronicsLog = new G4LogicalVolume(electronicsTub, copper, "ElectronicsLog", 0, 0, 0);

  G4VisAttributes *eVis = new G4VisAttributes();
  eVis->SetColour(200./255, 10./255, 10./255, 0.8);
  electronicsLog->SetVisAttributes(eVis);
  G4RotationMatrix * mat = new G4RotationMatrix();
  mat->rotateX(pi/2);
  new G4PVPlacement (mat, G4ThreeVector(0, _distance, 0), electronicsLog, "FakeElectronics", _parentPart->getContainerLogicalVolume(), false, 0);
  new G4PVPlacement (mat, G4ThreeVector(0, -_distance, 0), electronicsLog, "FakeElectronics", _parentPart->getContainerLogicalVolume(), false, 0);

  _partPhysicalVolume = 0;
  _partLogicalVolume = electronicsLog;
  _partContainerLogicalVolume = 0;
  return true;
}



