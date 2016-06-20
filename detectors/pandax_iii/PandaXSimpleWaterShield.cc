#include "detector/PandaXSimpleWaterShield.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"
#include "analysis/PandaXSensitiveDetector.hh"

#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4SDManager.hh>
#include <G4VSDFilter.hh>
#include <G4SDParticleFilter.hh>

#include <G4VisAttributes.hh>

#include <math.h>
// anonymous namespace to register the PandaXSimpleWaterShield

namespace {

  BambooDetectorPart * createPandaXSimpleWaterShield (const G4String & name)
  {
    return new PandaXSimpleWaterShield(name);
  }

  const std::string PandaXSimpleWaterShieldName("PandaXSimpleWaterShield");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXSimpleWaterShieldName, createPandaXSimpleWaterShield);
}

PandaXSimpleWaterShield::PandaXSimpleWaterShield (const G4String & name)
  : BambooDetectorPart(name)
{

  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);
  _length = BambooUtils::evaluate(dp.getParameterAsString("length"));
  _width = BambooUtils::evaluate(dp.getParameterAsString("width"));
  _height = BambooUtils::evaluate(dp.getParameterAsString("height"));
  if (_length == 0) {
    _length = 5.0 * m;
  }
  if (_width == 0) {
    _width = 5.0 * m;
  }
  if (_height == 0) {
    _height = 10.0 * m;
  }

  _isVisible = false;
  int iV = dp.getParameterAsInt("visible");
  if (iV == 1) {
    _isVisible = true;
  }
  _countingFluxIn = false;
  int cFi = dp.getParameterAsInt("counting_flux_in");
  if (cFi == 1) {
    _countingFluxIn = true;
  }
  G4cout << "PandaXSimpleWaterShield found..." << G4endl;
}

G4bool PandaXSimpleWaterShield::construct ()
{
  // add construction code here ...

  G4Material * lowCarbonSteel = G4Material::GetMaterial("Low_Carbon_Steel");
  double steelThickness = 1.0*cm;
  G4Box * carbonSteelBox = new G4Box("CarbonSteelBox", _length/2.+steelThickness, _width/2.+steelThickness, _height/2.+steelThickness);
  _partLogicalVolume = new G4LogicalVolume(carbonSteelBox, lowCarbonSteel, "CarbonSteelTankLog", 0, 0, 0);
  _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "CarbonSteelTank", _parentPart->getContainerLogicalVolume(), false, 0);
  G4VisAttributes * carbonSteelTankVisAtt = new G4VisAttributes(G4Colour(0.8, 0.8, 0.7, 0.5));
  carbonSteelTankVisAtt->SetVisibility(_isVisible);
  _partLogicalVolume->SetVisAttributes(carbonSteelTankVisAtt);

  G4Material * water = G4Material::GetMaterial("G4_WATER");
  G4Box * waterShieldBox = new G4Box("PandaXSimpleWaterShieldBox", _length/2., _width/2., _height/2.);
  G4LogicalVolume * waterShieldLog = new G4LogicalVolume(waterShieldBox, water, "PandaXSimpleWaterShieldLog", 0, 0, 0);

  G4VisAttributes * waterShieldVisAtt = new G4VisAttributes(G4Colour(0., 0.9, 0.2, 0.5));
  waterShieldVisAtt->SetVisibility(_isVisible);
  waterShieldLog->SetVisAttributes(waterShieldVisAtt);

  G4VPhysicalVolume * waterShield = new G4PVPlacement(0, G4ThreeVector(), waterShieldLog, "PandaXSimpleWaterShield", _partLogicalVolume, false, 0);
  G4cout << getName() << ": " << waterShield->GetName() << " created." << G4endl;
  _partContainerLogicalVolume = waterShieldLog;

  if (_countingFluxIn) {
    PandaXSensitiveDetector * waterSD = new PandaXSensitiveDetector("WaterSD", false, true);
    G4SDManager * sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(waterSD);
    waterShieldLog->SetSensitiveDetector(waterSD);
    G4cout << "Enable PandaXSensitiveDetector for Water." << G4endl;
    G4VSDFilter * gammaFilter = new G4SDParticleFilter("gammaFilter");
    ((G4SDParticleFilter *) gammaFilter )->add("gamma");
    waterSD->SetFilter(gammaFilter);
    G4cout << G4endl;
  }
  return true;
}

