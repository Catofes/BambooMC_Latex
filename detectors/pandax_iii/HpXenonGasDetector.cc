#include "detector/HpXenonGasDetector.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "BambooUtils.hh"

#include <G4Isotope.hh>
#include <G4Element.hh>
#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4Tubs.hh>
#include <G4SDManager.hh>
#include <G4UniformElectricField.hh>
#include <G4FieldManager.hh>
#include <G4EqMagElectricField.hh>
#include <G4ClassicalRK4.hh>
#include <G4MagIntegratorStepper.hh>
#include <G4MagIntegratorDriver.hh>
#include <G4ChordFinder.hh>

// anonymous namespace to register the HpXenonGasDetector

namespace {

  BambooDetectorPart * createHpXenonGasDetector (const G4String & name)
  {
    return new HpXenonGasDetector(name);
  }

  const std::string HpXenonGasDetectorType("HpXenonGasDetector");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(HpXenonGasDetectorType, createHpXenonGasDetector);
}

HpXenonGasDetector::HpXenonGasDetector (const G4String & name)
  : BambooDetectorPart(name)
{
  //  BambooGlobalVariables *bgv = BambooGlobalVariables::Instance();

  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);

  _vesselInnerRadius = BambooUtils::evaluate(dp.getParameterAsString("vessel_inner_radius"));
  _vesselBarrelThickness = BambooUtils::evaluate(dp.getParameterAsString("vessel_barrel_thickness"));
  _vesselEndThickness = BambooUtils::evaluate(dp.getParameterAsString("vessel_end_thickness"));

  _vesselInnerHeight = BambooUtils::evaluate(dp.getParameterAsString("vessel_inner_height"));

  int is_sensitive = dp.getParameterAsInt("is_sensitive");

  _shiftX = BambooUtils::evaluate(dp.getParameterAsString("shift_x"));
  _shiftY = BambooUtils::evaluate(dp.getParameterAsString("shift_y"));
  _shiftZ = BambooUtils::evaluate(dp.getParameterAsString("shift_z"));

  if (_vesselInnerRadius <= 0) {
    _vesselInnerRadius = 750 * mm;
  }
  if (_vesselInnerHeight <= 0) {
    _vesselInnerHeight = 2000.* mm;
  }
  if (_vesselBarrelThickness <=0) {
    _vesselBarrelThickness = 10. * mm;
  }
  if (_vesselEndThickness <=0) {
    _vesselEndThickness = 10. * mm;
  }
  if (_shiftX == 0) {
    _shiftX = 0 * mm;
  }
  if (_shiftY == 0) {
    _shiftY = 0 * mm;
  }
  if (_shiftZ == 0) {
    _shiftZ = 0 * mm;
  }

  if (is_sensitive == 1) {
    _isSensitive = true;
  } else {
    _isSensitive = false;
  }

  if (is_sensitive != 1 && is_sensitive != 0) {
    G4cout << "is_sensitive should be set to either 0 or 1. 0 will be used as default." << G4endl;
  }

  G4cout << "HpXenonGasDetector found..." << G4endl;
}


G4bool HpXenonGasDetector::construct()
{
  G4Material * copper = G4Material::GetMaterial("G4_Cu");
  G4VSolid * vesselTub = new G4Tubs("CopperVesselTub", 0, _vesselInnerRadius + _vesselBarrelThickness, _vesselInnerHeight/2. + _vesselEndThickness, 0., 2.*pi);

  _copperVesselLogicalVolume = new G4LogicalVolume(vesselTub, copper, "CopperVesselLog", 0, 0, 0);

  G4RotationMatrix * mat = new G4RotationMatrix();
  mat->rotateX(pi/2);

  _copperVesselPhysicalVolume = new G4PVPlacement(mat, G4ThreeVector(_shiftX, _shiftY, _shiftZ), _copperVesselLogicalVolume, "CopperVessel", _parentPart->getContainerLogicalVolume(), false, 0);

  G4VisAttributes * vesselVis = new G4VisAttributes();
  vesselVis->SetColour(224./255, 126./255, 11./255, 0.4);
  //  vesselVis->SetVisibility(_isVisible);
  _copperVesselLogicalVolume->SetVisAttributes(vesselVis);


  // create the xenon detector

  G4Material *XeTMAMixture = G4Material::GetMaterial("XeTMAMixture");
  G4VSolid *XeTub = new G4Tubs("XeTub", 0, _vesselInnerRadius, _vesselInnerHeight / 2., 0, 2 * pi);
  _XeTubLogicalVolume = new G4LogicalVolume(XeTub, XeTMAMixture, "XeTubLog", 0, 0, 0);

  _XeTubPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _XeTubLogicalVolume, "GasXenon",
                                           _copperVesselLogicalVolume, false, 0);

  _partLogicalVolume = _copperVesselLogicalVolume;
  _partPhysicalVolume = _copperVesselPhysicalVolume;
  _partContainerLogicalVolume = _XeTubLogicalVolume;

  if (_isSensitive){
    PandaXSensitiveDetector *hpXeSD = new PandaXSensitiveDetector("HpXeSD");
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(hpXeSD);
    _XeTubLogicalVolume->SetSensitiveDetector(hpXeSD);
  }

  return true;
}
