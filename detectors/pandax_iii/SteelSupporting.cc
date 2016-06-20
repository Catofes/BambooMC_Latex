#include "detector/SteelSupporting.hh"
#include "detector/BambooDetectorFactory.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>

#include <G4VisAttributes.hh>

namespace {

  BambooDetectorPart * createSteelSupporting (const G4String & name)
  {
    return new SteelSupporting(name);
  }

  const std::string SteelSupportingName("SteelSupporting");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(SteelSupportingName, createSteelSupporting);
}

SteelSupporting::SteelSupporting (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters("SteelSupporting");
  _shiftX = dp.getParameterAsDouble("shiftX") * mm;
  _shiftY = dp.getParameterAsDouble("shiftY") * mm;
  _shiftZ = dp.getParameterAsDouble("shiftZ") * mm;
}

G4bool SteelSupporting::construct ()
{
  // stainless steel
  G4Material* stainlessSteel = G4Material::GetMaterial("G4_STAINLESS-STEEL");

  G4Box * steelHBarX = new G4Box("steelHBarX", 100 * mm, 3900 * mm, 100 * mm);
  G4LogicalVolume * steelHBarXLV = new G4LogicalVolume(steelHBarX, stainlessSteel, "steelHBarXLV");
  G4Box * steelHBarY = new G4Box("steelHBarY", 8100 * mm, 100 * mm, 100 * mm);
  G4LogicalVolume * steelHBarYLV = new G4LogicalVolume(steelHBarY, stainlessSteel, "steelHBarYLV");

  G4Box * steelLeg = new G4Box("steelLeg", 100 * mm, 100 * mm, 2400*mm);
  G4LogicalVolume * steelLegLV = new G4LogicalVolume(steelLeg, stainlessSteel, "steelLegLV");

  G4Box * steelPillar = new G4Box("steelPillar", 100 * mm, 100 * mm, 1000*mm);
  G4LogicalVolume * steelPillarLV = new G4LogicalVolume(steelPillar, stainlessSteel, "steelPillarLV");

  G4VisAttributes * steelVisAtt = new G4VisAttributes(G4Colour(0.8, 0.8, 0.7, 0.5));
  steelVisAtt->SetVisibility(true);
  steelHBarXLV->SetVisAttributes(steelVisAtt);
  steelHBarYLV->SetVisAttributes(steelVisAtt);
  steelLegLV->SetVisAttributes(steelVisAtt);
  steelPillarLV->SetVisAttributes(steelVisAtt);

  std::vector<double> xpos = {-8000, -4500, -1500, 1500, 4500, 8000};
  for (int i=0; i<6; ++i) {
    new G4PVPlacement(0, G4ThreeVector(xpos[i]*mm, 0, -1.6*m), steelHBarXLV, "SteelHBarX", _parentPart->getContainerLogicalVolume(), true, i);
  }

  std::vector<double> ypos = {-4000, 4000};
  for (int i=0; i<2; ++i) {
    new G4PVPlacement(0, G4ThreeVector(0, ypos[i]*mm, -1.6*m), steelHBarYLV, "SteelHBarY", _parentPart->getContainerLogicalVolume(), true, i);
  }

  for (int i=0; i<6; ++i) {
    for (int j=0; j<2; ++j) {
      new G4PVPlacement(0, G4ThreeVector(xpos[i]*mm, ypos[j]*mm, -4.1*m), steelLegLV, "SteelLeg", _parentPart->getContainerLogicalVolume(), true, i*2+j);
    }
  }

  for (int i=0; i<2; ++i) {
    for (int j=0; j<2; ++j) {
      new G4PVPlacement(0, G4ThreeVector(xpos[5-i]*mm, ypos[j]*mm, -0.5*m), steelPillarLV, "SteelPillar", _parentPart->getContainerLogicalVolume(), true, i*2+j);
    }
  }
return true;
}

