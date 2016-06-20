#include "detector/WaterShield.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"
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
// anonymous namespace to register the WaterShield

namespace {

  BambooDetectorPart * createWaterShield (const G4String & name)
  {
    return new WaterShield(name);
  }

  const std::string WaterShieldName("WaterShield");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(WaterShieldName, createWaterShield);
}

WaterShield::WaterShield (const G4String & name)
  : BambooDetectorPart(name)
{
  
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);
  _shape = dp.getParameterAsInt("shape");
  if (_shape == 1) {
    _length = dp.getParameterAsDouble("length") * m;
    _width = dp.getParameterAsDouble("width") * m;
    _height = dp.getParameterAsDouble("height") * m;
    if (_length == 0) {
      _length = 5.0 * m;
    }
    if (_width == 0) {
      _width = 5.0 * m;
    }
    if (_height == 0) {
      _height = 10.0 * m;
    }
  } else if (_shape == 2) {
    _radius = dp.getParameterAsDouble("radius") * m;
    _height = dp.getParameterAsDouble("height") * m;
    if (_radius == 0) {
      _radius = 5.0 * m;
    }
    if (_height == 0) {
      _height = 10.0 * m;
    }
  } else {
    _shape = 1;
    _length = 5.0 * m;
    _width = 5.0 * m;
    _height = 10.0 * m;
  }

  _countingFluxIn = false;
  if (_shape == 1) {
    int cFi = dp.getParameterAsInt("counting_flux_in");
    if (cFi == 1) {
      _countingFluxIn = true;
    }
  }
  G4cout << "WaterShield found..." << G4endl;
}

G4bool WaterShield::construct ()
{
  // add construction code here ...
  G4Material * water = G4Material::GetMaterial("G4_WATER");
  _partLogicalVolume = 0;
  if (_shape == 1) {
    G4Box * waterShieldBox = new G4Box("WaterShieldBox", _length/2., _width/2., _height/2.);
    _partLogicalVolume = new G4LogicalVolume(waterShieldBox, water, "WaterShieldLog", 0, 0, 0);
  } else if (_shape == 2) {
    G4Tubs * waterShieldTub = new G4Tubs("WaterShieldTub", 0, _radius, _height/2., 0., 2. * M_PI);
    _partLogicalVolume = new G4LogicalVolume(waterShieldTub, water, "WaterShieldLog", 0, 0, 0);
  }
  G4VisAttributes * waterShieldVisAtt = new G4VisAttributes(G4Colour(0., 0., 0.8, 0.8));
  _partLogicalVolume->SetVisAttributes(waterShieldVisAtt);
  _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "WaterShield", _parentPart->getContainerLogicalVolume(), false, 0);
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

