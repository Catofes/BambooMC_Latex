#include "detector/PandaXLab.hh"
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
#include <G4SDManager.hh>
#include <G4VSDFilter.hh>
#include <G4SDParticleFilter.hh>
#include <G4VisAttributes.hh>

// anonymous namespace to register the PandaXLab

namespace {

  BambooDetectorPart * createPandaXLab ()
  {
    return new PandaXLab("PandaXLab");
  }

  const std::string PandaXLabName("PandaXLab");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXLabName, createPandaXLab);
}

PandaXLab::PandaXLab (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters("PandaXLab");

  _length = BambooUtils::evaluate(dp.getParameterAsString("length"));
  _width = BambooUtils::evaluate(dp.getParameterAsString("width"));
  _height = BambooUtils::evaluate(dp.getParameterAsString("height"));

  _rockWallThickness = BambooUtils::evaluate(dp.getParameterAsString("rock_wall_thickness"));
  _rockRoofThickness = BambooUtils::evaluate(dp.getParameterAsString("rock_roof_thickness"));
  _concreteWallThickness = BambooUtils::evaluate(dp.getParameterAsString("concrete_wall_thickness"));
  _concreteRoofThickness = BambooUtils::evaluate(dp.getParameterAsString("concrete_roof_thickness"));
  _concreteFloorThickness = BambooUtils::evaluate(dp.getParameterAsString("concrete_floor_thickness"));

  if (_length == 0) {
    _length = 10000. * mm;
  }
  if (_width == 0) {
    _width = 5000. * mm;
  }
  if (_height == 0) {
    _height = 6000. * mm;
  }
  if (_rockWallThickness == 0) {
    _rockWallThickness = 2000. * mm;
  }
  if (_rockRoofThickness == 0) {
    _rockRoofThickness = 2000. * mm;
  }
  if (_rockFloorThickness == 0) {
    _rockFloorThickness = 2000. * mm;
  }
  if (_concreteWallThickness == 0) {
    _concreteWallThickness = 300. * mm;
  }
  if (_concreteRoofThickness == 0) {
    _concreteRoofThickness = 300. * mm;
  }
  if (_concreteFloorThickness == 0) {
    _concreteFloorThickness = 300. * mm;
  }

  _rockFloorThickness = 0;
  _isVisible = false;
  int iV = dp.getParameterAsInt("visible");
  if (iV == 1) {
    _isVisible = true;
  }
  _countAirFlux = false;
  int caF = dp.getParameterAsInt("count_air_flux");
  if (caF == 1) {
    _countAirFlux = true;
  }
  _sensitiveAir = _countAirFlux;
  G4cout << "PandaXLab found..." << G4endl;
}

G4bool PandaXLab::construct ()
{
  double concreteLength = _length + 2. * _concreteWallThickness;
  double concreteWidth = _width + 2. * _concreteWallThickness;
  double concreteHeight = _height +  _concreteFloorThickness + _concreteRoofThickness;
  
  double rockLength = concreteLength + 2. * _rockWallThickness;
  double rockWidth = concreteWidth + 2. * _rockWallThickness;
  double rockHeight = (concreteHeight - _concreteFloorThickness + _rockRoofThickness) * 2.0;

  _rockFloorThickness = rockHeight/2.0 - _concreteFloorThickness;

  // the rock wall
  G4Material * marble = G4Material::GetMaterial("MARBLE");
  if (marble == 0) {
    G4cout << "Can't find the material MARBLE" << G4endl;
    return false;
  }

  G4Box * marbleBox = new G4Box("MarbleBox", rockLength/2., rockWidth/2., rockHeight/2.);
  _rockWallLogicalVolume = new G4LogicalVolume(marbleBox, marble, "MarbleBoxLog", 0, 0, 0);
  _partLogicalVolume = _rockWallLogicalVolume;

  G4VisAttributes * rockVis = new G4VisAttributes();
  rockVis->SetColour(0.6, 0.6, 0.6, 0.2);
  rockVis->SetVisibility(_isVisible);
  _rockWallLogicalVolume->SetVisAttributes(rockVis);

  // the PandaXLab provide the world volume!
  _rockWallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "RockWall", 0, false, 0);
  _partPhysicalVolume = _rockWallPhysicalVolume;

  // the concrete wall
  G4Material * concrete = G4Material::GetMaterial("G4_CONCRETE");
  G4Box * concreteBox = new G4Box("ConcreteBox", concreteLength/2., concreteWidth/2., concreteHeight/2.);
  _concreteWallLogicalVolume = new G4LogicalVolume(concreteBox, concrete, "ConcreteBoxLog", 0, 0, 0);
  _concreteWallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5*concreteHeight-_concreteFloorThickness), _concreteWallLogicalVolume, "ConcreteWall", _rockWallLogicalVolume, false, 0);

  G4VisAttributes * concreteVis = new G4VisAttributes();
  concreteVis->SetColour(0.9, 0.9, 0.9, 0.2);
  concreteVis->SetVisibility(_isVisible);
  _concreteWallLogicalVolume->SetVisAttributes(concreteVis);

  // the inner space
  G4Material * air = G4Material::GetMaterial("G4_AIR");
  G4Box * innerSpaceBox = new G4Box("InnerSpaceBox", _length/2., _width/2., _height/2.);
  _innerSpaceLogicalVolume = new G4LogicalVolume(innerSpaceBox, air, "InnerSpaceLog", 0, 0, 0);
  _innerSpacePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5*(_concreteFloorThickness-_concreteRoofThickness)), _innerSpaceLogicalVolume, "InnerSpaceWall", _concreteWallLogicalVolume, false, 0);

  if (_sensitiveAir) {
    PandaXSensitiveDetector * spaceSD = new PandaXSensitiveDetector("LabSpaceSD", false, true);
    G4SDManager * sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(spaceSD);
    _innerSpaceLogicalVolume->SetSensitiveDetector(spaceSD);
    G4cout << "Enable PandaXSensitiveDetector for Air." << G4endl;
    G4VSDFilter * gammaFilter = new G4SDParticleFilter("gammaFilter");
    ((G4SDParticleFilter *)gammaFilter)->add("gamma");
    spaceSD->SetFilter(gammaFilter);
    G4cout << G4endl;
  }
  G4VisAttributes * spaceVis = new G4VisAttributes();
  spaceVis->SetColour(0.98, 0.98, 0.98, 0.1);
  spaceVis->SetVisibility(_isVisible);
  _innerSpaceLogicalVolume->SetVisAttributes(spaceVis);

  _partContainerLogicalVolume = _innerSpaceLogicalVolume;
  return true;
}

