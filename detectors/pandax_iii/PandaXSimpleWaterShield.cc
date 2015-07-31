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

  BambooDetectorPart * createPandaXSimpleWaterShield ()
  {
    return new PandaXSimpleWaterShield("PandaXSimpleWaterShield");
  }

  const std::string PandaXSimpleWaterShieldName("PandaXSimpleWaterShield");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXSimpleWaterShieldName, createPandaXSimpleWaterShield);
}

PandaXSimpleWaterShield::PandaXSimpleWaterShield (const G4String & name)
  : BambooDetectorPart(name)
{
  
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters("PandaXSimpleWaterShield");
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
  G4Material * water = G4Material::GetMaterial("G4_WATER");
  _partLogicalVolume = 0;

  G4Box * waterShieldBox = new G4Box("PandaXSimpleWaterShieldBox", _length/2., _width/2., _height/2.);
  _partLogicalVolume = new G4LogicalVolume(waterShieldBox, water, "PandaXSimpleWaterShieldLog", 0, 0, 0);

  G4VisAttributes * waterShieldVisAtt = new G4VisAttributes(G4Colour(0., 0.9, 0.2, 0.5));
  waterShieldVisAtt->SetVisibility(_isVisible);
  _partLogicalVolume->SetVisAttributes(waterShieldVisAtt);

  _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "PandaXSimpleWaterShield", _parentPart->getContainerLogicalVolume(), false, 0);
  _partContainerLogicalVolume = _partLogicalVolume;

  if (_countingFluxIn) {
    PandaXSensitiveDetector * waterSD = new PandaXSensitiveDetector("WaterSD", false, true);
    G4SDManager * sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(waterSD);
    _partLogicalVolume->SetSensitiveDetector(waterSD);
    G4cout << "Enable PandaXSensitiveDetector for Water." << G4endl;
    G4VSDFilter * gammaFilter = new G4SDParticleFilter("gammaFilter");
    ((G4SDParticleFilter *) gammaFilter )->add("gamma");
    waterSD->SetFilter(gammaFilter);
    G4cout << G4endl;
  }
  return true;
}

