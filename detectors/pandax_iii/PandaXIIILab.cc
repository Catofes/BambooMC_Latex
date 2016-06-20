#include "detector/PandaXIIILab.hh"
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
#include <G4UnionSolid.hh>
#include <G4SDManager.hh>
#include <G4VSDFilter.hh>
#include <G4SDParticleFilter.hh>
#include <G4VisAttributes.hh>

// anonymous namespace to register the PandaXIIILab

namespace {

  BambooDetectorPart * createPandaXIIILab (const G4String & name)
  {
    return new PandaXIIILab(name);
  }

  const std::string PandaXIIILabName("PandaXIIILab");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXIIILabName, createPandaXIIILab);
}

PandaXIIILab::PandaXIIILab (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);

  _length = BambooUtils::evaluate(dp.getParameterAsString("length"));
  _width = BambooUtils::evaluate(dp.getParameterAsString("width"));
  _height = BambooUtils::evaluate(dp.getParameterAsString("height"));

  _rockWallThickness = BambooUtils::evaluate(dp.getParameterAsString("rock_wall_thickness"));
  _rockRoofThickness = BambooUtils::evaluate(dp.getParameterAsString("rock_roof_thickness"));
  _concreteWallThickness = BambooUtils::evaluate(dp.getParameterAsString("concrete_wall_thickness"));
  _concreteRoofThickness = BambooUtils::evaluate(dp.getParameterAsString("concrete_roof_thickness"));
  _concreteFloorThickness = BambooUtils::evaluate(dp.getParameterAsString("concrete_floor_thickness"));

  _platformHeight = BambooUtils::evaluate(dp.getParameterAsString("platform_height"));
  _platformLength = BambooUtils::evaluate(dp.getParameterAsString("platform_length"));

  _poolLength = BambooUtils::evaluate(dp.getParameterAsString("water_pool_length"));
  _poolWidth = BambooUtils::evaluate(dp.getParameterAsString("water_pool_width"));
  _poolDepth = BambooUtils::evaluate(dp.getParameterAsString("water_pool_depth"));

  if (_length == 0) {
    _length = 130. * m;
  }
  if (_width == 0) {
    _width = 14. * m;
  }
  if (_height == 0) {
    _height = 14. * m;
  }
  if (_rockWallThickness == 0) {
    _rockWallThickness = 2. * m;
  }
  if (_rockRoofThickness == 0) {
    _rockRoofThickness = 2. * m;
  }
  if (_rockFloorThickness == 0) {
    _rockFloorThickness = 2. * m;
  }
  if (_concreteWallThickness == 0) {
    _concreteWallThickness = 1. * m;
  }
  if (_concreteRoofThickness == 0) {
    _concreteRoofThickness = 1. * m;
  }
  if (_concreteFloorThickness == 0) {
    _concreteFloorThickness = 0.5 * m;
  }
  if (_platformHeight == 0) {
    _platformHeight = 6. * m;
  }
  if (_platformLength == 0) {
    _platformLength = 40. * m;
  }
  if (_poolLength == 0) {
    _poolLength = 20.*m;
  }
  if (_poolWidth == 0) {
    _poolWidth = 10. * m;
  }
  if (_poolDepth == 0) {
    _poolDepth = 10. * m;
  }
  // TODO: need to check the platform height and length;
  
  _rockFloorThickness = 0;
  _isVisible = false;
  int iV = dp.getParameterAsInt("visible");
  if (iV == 1) {
    _isVisible = true;
  }
  _countWaterFlux = false;
  int caF = dp.getParameterAsInt("count_water_flux");
  if (caF == 1) {
    _countWaterFlux = true;
  }
  _sensitiveWater = _countWaterFlux;
  G4cout << "PandaXIIILab found..." << G4endl;
}

G4bool PandaXIIILab::construct ()
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

  // the PandaXIIILab provide the world volume!
  _rockWallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "RockWall", 0, false, 0);
  _partPhysicalVolume = _rockWallPhysicalVolume;


  // the concrete wall
  G4Material * concrete = G4Material::GetMaterial("G4_CONCRETE");

  G4Box * concreteBox_part1 = new G4Box("ConcreteBox_Part1", (concreteLength-_platformLength)/2., concreteWidth/2., concreteHeight/2.);
  G4Box * concreteBox_part2 = new G4Box("ConcreteBox_Part2", (_platformLength+_concreteWallThickness)/2., concreteWidth/2., (concreteHeight-_platformHeight)/2.);
  G4Box * concreteBox_part3 = new G4Box("ConcreteBox_Part3", 0.5*_poolLength+_concreteWallThickness, 0.5*_poolWidth+_concreteWallThickness, 0.5*(_poolDepth+_concreteFloorThickness));
  G4UnionSolid * concreteUnion1 = new G4UnionSolid("ConcreteUnion1", concreteBox_part1, concreteBox_part2, 0, G4ThreeVector(0.5*(concreteLength-_concreteWallThickness), 0, 0.5*_platformHeight));
  G4UnionSolid * concreteUnion2 = new G4UnionSolid("ConcreteUnion2", concreteUnion1, concreteBox_part3, 0, G4ThreeVector(0.5*(concreteLength-_concreteWallThickness), 0, _platformHeight-0.5*(_poolDepth+concreteHeight-_concreteFloorThickness)));
  _concreteWallLogicalVolume = new G4LogicalVolume(concreteUnion2, concrete, "ConcreteBoxLog", 0, 0, 0);
  _concreteWallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(-0.5*_platformLength, 0, 0.5*concreteHeight-_concreteFloorThickness), _concreteWallLogicalVolume, "ConcreteWall", _rockWallLogicalVolume, false, 0);

  G4VisAttributes * concreteVis = new G4VisAttributes();
  concreteVis->SetColour(0.9, 0.9, 0.9, 0.2);
  concreteVis->SetVisibility(_isVisible);
  _concreteWallLogicalVolume->SetVisAttributes(concreteVis);

  // the inner space
  G4Material * air = G4Material::GetMaterial("G4_AIR");

  G4Box * innerSpaceBox1 = new G4Box("InnerSpaceBox1", (_length-_platformLength)/2., _width/2., _height/2.); // the large one

  G4Box * innerSpaceBox2 = new G4Box("InnerSpaceBox2", _platformLength/2.+2*mm, _width/2., (_height-_platformHeight)/2.); // the one above the platform

  // // move the 2nd box upwards
  G4ThreeVector iZTrans(0.5*_length-2*mm, 0, 0.5*_platformHeight);

  G4UnionSolid * innerSpaceSolid = new G4UnionSolid("InnerSpace1+2", innerSpaceBox1, innerSpaceBox2, 0, iZTrans);
  _innerSpaceLogicalVolume = new G4LogicalVolume(innerSpaceSolid, air, "InnerSpaceLog", 0, 0, 0);
  _innerSpacePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5*(_concreteFloorThickness-_concreteRoofThickness)), _innerSpaceLogicalVolume, "InnerSpaceWall", _concreteWallLogicalVolume, false, 0);

  G4VisAttributes * spaceVis = new G4VisAttributes();
  spaceVis->SetColour(0.98, 0.98, 0.98, 0.1);
  spaceVis->SetVisibility(_isVisible);
  _innerSpaceLogicalVolume->SetVisAttributes(spaceVis);

  // The water pool
  G4Material * water = G4Material::GetMaterial("G4_WATER");
  G4Box * waterPoolBox = new G4Box("WaterPoolBox", 0.5*_poolLength, 0.5*_poolWidth, 0.5*_poolDepth);
  _waterPoolLogicalVolume = new G4LogicalVolume(waterPoolBox, water, "WaterPoolLog", 0, 0, 0);
  _waterPoolPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0.5*(concreteLength-_concreteWallThickness), 0, _platformHeight-0.5*(_poolDepth+concreteHeight)+_concreteFloorThickness), _waterPoolLogicalVolume, "WaterPool", _concreteWallLogicalVolume, false, 0);

  G4VisAttributes * waterVis = new G4VisAttributes();
  waterVis->SetColour(0.1, 0.1, 0.8, 0.2);
  waterVis->SetVisibility(_isVisible);
  _waterPoolLogicalVolume->SetVisAttributes(waterVis);

  if (_sensitiveWater) {
    PandaXSensitiveDetector * waterSD = new PandaXSensitiveDetector("WaterSD", false, true);
    G4SDManager * sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(waterSD);
    _waterPoolLogicalVolume->SetSensitiveDetector(waterSD);
    G4cout << "Enable PandaXSensitiveDetector for Water." << G4endl;
    G4VSDFilter * gammaFilter = new G4SDParticleFilter("gammaFilter");
    ((G4SDParticleFilter *)gammaFilter)->add("gamma");
    waterSD->SetFilter(gammaFilter);
    G4cout << G4endl;
  }

  _partContainerLogicalVolume = _waterPoolLogicalVolume;

  G4cout << "Concrete Mass: " << (_concreteWallLogicalVolume->GetMass()-_waterPoolLogicalVolume->GetMass()-_innerSpaceLogicalVolume->GetMass())/kg << " kg." << G4endl;

  return true;
}

