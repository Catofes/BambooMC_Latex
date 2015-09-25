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

  BambooDetectorPart * createHpXenonGasDetector ()
  {
    return new HpXenonGasDetector("HpXenonGasDetector");
  }

  const std::string HpXenonGasDetectorName("HpXenonGasDetector");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(HpXenonGasDetectorName, createHpXenonGasDetector);
}

HpXenonGasDetector::HpXenonGasDetector (const G4String & name)
  : BambooDetectorPart(name)
{
  //  BambooGlobalVariables *bgv = BambooGlobalVariables::Instance();

  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters("HpXenonGasDetector");

  _vesselInnerRadius = BambooUtils::evaluate(dp.getParameterAsString("vessel_inner_radius"));
  _vesselBarrelThickness = BambooUtils::evaluate(dp.getParameterAsString("vessel_barrel_thickness"));
  _vesselEndThickness = BambooUtils::evaluate(dp.getParameterAsString("vessel_end_thickness"));

  _vesselInnerHeight = BambooUtils::evaluate(dp.getParameterAsString("vessel_inner_height"));

  _shiftX = BambooUtils::evaluate(dp.getParameterAsString("shift_x"));
  _shiftY = BambooUtils::evaluate(dp.getParameterAsString("shift_y"));
  _shiftZ = BambooUtils::evaluate(dp.getParameterAsString("shift_z"));

  _electricFieldZ = BambooUtils::evaluate(dp.getParameterAsString("electric_field_z"));

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

  if (_electricFieldZ>0) {
    _electricFieldZ = _electricFieldZ * volt / cm;
  } else {
    _electricFieldZ = 0;
  }

  G4cout << "HpXenonGasDetector found..." << G4endl;
}


G4bool HpXenonGasDetector::construct()
{
  G4Material * copper = G4Material::GetMaterial("G4_Cu");
  G4VSolid * vesselTub = new G4Tubs("CopperVesselTub", 0, _vesselInnerRadius + _vesselBarrelThickness, _vesselInnerHeight/2. + _vesselEndThickness, 0., 2.*pi);

  _copperVesselLogicalVolume = new G4LogicalVolume(vesselTub, copper, "CopperVesselLog", 0, 0, 0);

  G4RotationMatrix * mat = new G4RotationMatrix();
  mat->rotateY(pi/2);
  
  _copperVesselPhysicalVolume = new G4PVPlacement(mat, G4ThreeVector(_shiftX, _shiftY, _shiftZ), _copperVesselLogicalVolume, "CopperVessel", _parentPart->getContainerLogicalVolume(), false, 0);

  G4VisAttributes * vesselVis = new G4VisAttributes();
  vesselVis->SetColour(224./255, 126./255, 11./255, 0.4);
  //  vesselVis->SetVisibility(_isVisible);
  _copperVesselLogicalVolume->SetVisAttributes(vesselVis);


    
  _partLogicalVolume = _copperVesselLogicalVolume;
  _partPhysicalVolume = _copperVesselPhysicalVolume;

  // create the xenon detector
  bool top;
  createXenonVolume(top = true);
  createXenonVolume(top = false);
  _partContainerLogicalVolume = 0;
  PandaXSensitiveDetector * hpXeSD = new PandaXSensitiveDetector("HpXeSD");
  G4SDManager * sdManager = G4SDManager::GetSDMpointer();
  sdManager->AddNewDetector(hpXeSD);
  _hpXenonLogicalVolumeTop->SetSensitiveDetector(hpXeSD);
  _hpXenonLogicalVolumeBottom->SetSensitiveDetector(hpXeSD);

  return true;
}

void HpXenonGasDetector::createXenonVolume (bool top)
{

  double gasTubRadius = _vesselInnerRadius;
  double gasHeight = _vesselInnerHeight;
  G4VSolid * xenonTub = new G4Tubs("CopperVesselTub", 0, gasTubRadius, gasHeight/4., 0., 2.*pi);
  G4String name("HighPressureXenonLog");
  double factor = 1;
  G4LogicalVolume * hpLV;
  if (top) {
    name += "Top";   
  } else {
    name += "Bottom";
    factor = -1;
  }
  G4Material * XeTMAMixture = G4Material::GetMaterial("XeTMAMixture");
  hpLV = new G4LogicalVolume(xenonTub, XeTMAMixture, name, 0, 0, 0);

  if (_electricFieldZ!=0) {
    G4Field * electricField = new G4UniformElectricField(G4ThreeVector(0, 0, -factor*_electricFieldZ));
    G4EqMagElectricField * equation = new G4EqMagElectricField((G4ElectroMagneticField*)electricField);
    G4MagIntegratorStepper * stepper = new G4ClassicalRK4(equation, 8);
    G4FieldManager * localFieldManager = new G4FieldManager(electricField);
    G4MagInt_Driver * driver = new G4MagInt_Driver(0.01*mm, stepper, stepper->GetNumberOfVariables());
    G4ChordFinder * chordFinder = new G4ChordFinder(driver);
    localFieldManager->SetChordFinder(chordFinder);
    hpLV->SetFieldManager(localFieldManager, true);
  }

  G4VPhysicalVolume * pV;

  G4RotationMatrix * mat = new G4RotationMatrix();
  if (top) {
    mat->rotateX(pi);
  }
  pV = new G4PVPlacement(mat, G4ThreeVector(0, 0, factor*gasHeight/4), hpLV, "HighPressureXenonTMA", _copperVesselLogicalVolume, false, 0);

  G4VisAttributes * xenonVis = new G4VisAttributes();
  xenonVis->SetColour(140./255, 140./255, 1., 0.4);
  hpLV->SetVisAttributes(xenonVis);

  if (top) {
    _hpXenonLogicalVolumeTop = hpLV;
    _hpXenonPhysicalVolumeTop = pV;
  } else {
    _hpXenonLogicalVolumeBottom = hpLV;
    _hpXenonPhysicalVolumeBottom = pV;
  }
}
