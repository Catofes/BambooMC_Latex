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
  G4cout << "HpXenonGasDetector found..." << G4endl;
}


G4bool HpXenonGasDetector::construct()
{
  // the enriched xenon gas
  G4Isotope * Xe136 = new G4Isotope ("Xe136", 54, 136, 135.907219 * g/mole);
  G4Isotope * Xe128 = new G4Isotope ("Xe128", 54, 128, 127.9035313 * g/mole);
  G4Isotope * Xe129 = new G4Isotope ("Xe129", 54, 129, 128.9047794 * g/mole);
  G4Isotope * Xe130 = new G4Isotope ("Xe130", 54, 130, 129.9035080 * g/mole);
  G4Isotope * Xe131 = new G4Isotope ("Xe131", 54, 131, 130.9050824 * g/mole);
  G4Isotope * Xe132 = new G4Isotope ("Xe132", 54, 132, 131.9041535 * g/mole);
  G4Isotope * Xe134 = new G4Isotope ("Xe134", 54, 134, 133.9053945 * g/mole);

  G4Element * enrichedXe = new G4Element("enriched Xe", "Xe", 7);
  enrichedXe->AddIsotope(Xe136, 0.8);
  enrichedXe->AddIsotope(Xe128, 0.004200157);
  enrichedXe->AddIsotope(Xe129, 0.05804977);
  enrichedXe->AddIsotope(Xe130, 0.008951335);
  enrichedXe->AddIsotope(Xe131, 0.046685906);
  enrichedXe->AddIsotope(Xe132, 0.059166763);
  enrichedXe->AddIsotope(Xe134, 0.022946069);

  G4cout << enrichedXe->GetA()/g*mole << G4endl;

  double density = 56.588 * kg/m3;

  G4Material * hpXe = new G4Material("High Pressure Xenon with Xe136 enriched", density, 1, kStateGas, _xenonTemperature, _xenonPressure);
  hpXe->AddElement(enrichedXe, 1.0);
  G4Material * copper = G4Material::GetMaterial("G4_Cu");
  G4VSolid * vesselTub = new G4Tubs("CopperVesselTub", 0, _vesselOuterRadius, _vesselHeight/2., 0., 2.*pi);

  _copperVesselLogicalVolume = new G4LogicalVolume(vesselTub, copper, "CopperVesselLog", 0, 0, 0);
  _copperVesselPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ), _copperVesselLogicalVolume, "CopperVessel", _parentPart->getContainerLogicalVolume(), false, 0);

  G4VisAttributes * vesselVis = new G4VisAttributes();
  vesselVis->SetColour(224./255, 126./255, 11./255, 0.4);
  //  vesselVis->SetVisibility(_isVisible);
  _copperVesselLogicalVolume->SetVisAttributes(vesselVis);

  double gasTubRadius = _vesselOuterRadius - _vesselBarrelThickness;
  double gasHeight = _vesselHeight - 2.* _vesselEndThickness;
  G4VSolid * xenonTub = new G4Tubs("CopperVesselTub", 0, gasTubRadius, gasHeight/2., 0., 2.*pi);
  _hpXenonLogicalVolume = new G4LogicalVolume(xenonTub, hpXe, "HighPressureXenonLog", 0, 0, 0);
  _hpXenonPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _hpXenonLogicalVolume, "HighPressureXenon", _copperVesselLogicalVolume, false, 0);
  G4VisAttributes * xenonVis = new G4VisAttributes();
  xenonVis->SetColour(140./255, 140./255, 1., 0.4);
  //  vesselVis->SetVisibility(_isVisible);
  _hpXenonLogicalVolume->SetVisAttributes(xenonVis);

    
  _partLogicalVolume = _copperVesselLogicalVolume;
  _partPhysicalVolume = _copperVesselPhysicalVolume;
  
  _partContainerLogicalVolume = _hpXenonLogicalVolume;
  PandaXSensitiveDetector * hpXeSD = new PandaXSensitiveDetector("HpXeSD");
  G4SDManager * sdManager = G4SDManager::GetSDMpointer();
  sdManager->AddNewDetector(hpXeSD);
  _hpXenonLogicalVolume->SetSensitiveDetector(hpXeSD);
  G4cout << "High Pressure Xenon Mass: " << _hpXenonLogicalVolume->GetMass()/kg << " kg." << G4endl;
  return true;
}



