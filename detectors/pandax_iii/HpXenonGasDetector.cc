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

  _vesselOuterRadius = BambooUtils::evaluate(dp.getParameterAsString("vessel_outer_radius"));
  _vesselBarrelThickness = BambooUtils::evaluate(dp.getParameterAsString("vessel_barrel_thickness"));
  _vesselEndThickness = BambooUtils::evaluate(dp.getParameterAsString("vessel_end_thickness"));

  _vesselHeight = BambooUtils::evaluate(dp.getParameterAsString("vessel_height"));

  _shiftX = BambooUtils::evaluate(dp.getParameterAsString("shift_x"));
  _shiftY = BambooUtils::evaluate(dp.getParameterAsString("shift_y"));
  _shiftZ = BambooUtils::evaluate(dp.getParameterAsString("shift_z"));

  _xenonPressure = BambooUtils::evaluate(dp.getParameterAsString("xenon_pressure"));
  _xenonTemperature = BambooUtils::evaluate(dp.getParameterAsString("xenon_temperature"));

  _electricFieldZ = BambooUtils::evaluate(dp.getParameterAsString("electric_field_z"));

  _xe136Fraction = BambooUtils::evaluate(dp.getParameterAsString("xe136_fraction"));

  if (_vesselOuterRadius <= 0) {
    _vesselOuterRadius = 760 * mm;
  }
  if (_vesselHeight <= 0) {
    _vesselHeight = 2020.* mm;
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

  if (_xenonTemperature <= 0) {
    _xenonTemperature = STP_Temperature;
  }

  if (_xenonPressure <= 0) {
    _xenonPressure = 10.0 * bar; 
  }

  if (_electricFieldZ>0) {
    _electricFieldZ = _electricFieldZ * volt / cm;
  } else {
    _electricFieldZ = 0;
  }

  if (_xe136Fraction <= 0) {
    _xe136Fraction = 0.8;
  }

  _hpXe = 0;
  G4cout << "HpXenonGasDetector found..." << G4endl;
}


G4bool HpXenonGasDetector::construct()
{
  G4Material * copper = G4Material::GetMaterial("G4_Cu");
  G4VSolid * vesselTub = new G4Tubs("CopperVesselTub", 0, _vesselOuterRadius, _vesselHeight/2., 0., 2.*pi);

  _copperVesselLogicalVolume = new G4LogicalVolume(vesselTub, copper, "CopperVesselLog", 0, 0, 0);
  _copperVesselPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ), _copperVesselLogicalVolume, "CopperVessel", _parentPart->getContainerLogicalVolume(), false, 0);

  G4VisAttributes * vesselVis = new G4VisAttributes();
  vesselVis->SetColour(224./255, 126./255, 11./255, 0.4);
  //  vesselVis->SetVisibility(_isVisible);
  _copperVesselLogicalVolume->SetVisAttributes(vesselVis);


    
  _partLogicalVolume = _copperVesselLogicalVolume;
  _partPhysicalVolume = _copperVesselPhysicalVolume;

  // create the xenon detector
  createEnrichedXenon();

  bool top;
  createXenonVolume(top = true);
  createXenonVolume(top = false);
  _partContainerLogicalVolume = 0;
  PandaXSensitiveDetector * hpXeSD = new PandaXSensitiveDetector("HpXeSD");
  G4SDManager * sdManager = G4SDManager::GetSDMpointer();
  sdManager->AddNewDetector(hpXeSD);
  _hpXenonLogicalVolumeTop->SetSensitiveDetector(hpXeSD);
  _hpXenonLogicalVolumeBottom->SetSensitiveDetector(hpXeSD);
  G4cout << "High Pressure Xenon Mass: " << _hpXenonLogicalVolumeTop->GetMass()/kg + _hpXenonLogicalVolumeBottom->GetMass()/kg << " kg." << G4endl;

  return true;
}

void HpXenonGasDetector::createEnrichedXenon()
{

  double xe128NaturalFraction = 0.019102;
  double xe129NaturalFraction = 0.264006;
  double xe130NaturalFraction = 0.04071;
  double xe131NaturalFraction = 0.212324;
  double xe132NaturalFraction = 0.269086;
  double xe134NaturalFraction = 0.104357;
  // the enriched xenon gas
  G4Isotope * Xe136 = new G4Isotope ("Xe136", 54, 136, 135.907219 * g/mole);
  G4Isotope * Xe128 = new G4Isotope ("Xe128", 54, 128, 127.9035313 * g/mole);
  G4Isotope * Xe129 = new G4Isotope ("Xe129", 54, 129, 128.9047794 * g/mole);
  G4Isotope * Xe130 = new G4Isotope ("Xe130", 54, 130, 129.9035080 * g/mole);
  G4Isotope * Xe131 = new G4Isotope ("Xe131", 54, 131, 130.9050824 * g/mole);
  G4Isotope * Xe132 = new G4Isotope ("Xe132", 54, 132, 131.9041535 * g/mole);
  G4Isotope * Xe134 = new G4Isotope ("Xe134", 54, 134, 133.9053945 * g/mole);

  G4Element * enrichedXe = new G4Element("enriched Xe", "Xe", 7);
  enrichedXe->AddIsotope(Xe136, _xe136Fraction);

  double restFraction = 1.0 - _xe136Fraction;
  enrichedXe->AddIsotope(Xe128, restFraction * xe128NaturalFraction);
  enrichedXe->AddIsotope(Xe129, restFraction * xe129NaturalFraction);
  enrichedXe->AddIsotope(Xe130, restFraction * xe130NaturalFraction);
  enrichedXe->AddIsotope(Xe131, restFraction * xe131NaturalFraction);
  enrichedXe->AddIsotope(Xe132, restFraction * xe132NaturalFraction);
  enrichedXe->AddIsotope(Xe134, restFraction * xe134NaturalFraction);

  G4cout << "Enriched Xe Atomic Mass: " << enrichedXe->GetA()/g*mole << " g/mole." << G4endl;

  double r = k_Boltzmann*Avogadro;
  double density = (enrichedXe->GetA())*_xenonPressure/_xenonTemperature/r;
  G4cout << "Enriched Xe Gas Density: " << density/kg*m3 << " kg/m3." << G4endl;
  _hpXe = new G4Material("High Pressure Xenon with Xe136 enriched", density, 1, kStateGas, _xenonTemperature, _xenonPressure);
  _hpXe->AddElement(enrichedXe, 1.0);
  
}

void HpXenonGasDetector::createXenonVolume (bool top)
{

  double gasTubRadius = _vesselOuterRadius - _vesselBarrelThickness;
  double gasHeight = _vesselHeight - 2.* _vesselEndThickness;
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
  hpLV = new G4LogicalVolume(xenonTub, _hpXe, name, 0, 0, 0);

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
  pV = new G4PVPlacement(mat, G4ThreeVector(0, 0, factor*gasHeight/4), hpLV, "HighPressureXenon", _copperVesselLogicalVolume, false, 0);

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
