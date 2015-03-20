#include "detector/PandaXLab.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>

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

  _length = dp.getParameterAsDouble("length") * mm;
  _width = dp.getParameterAsDouble("width") * mm;
  _height = dp.getParameterAsDouble("height") * mm;

  _rockWallThickness = dp.getParameterAsDouble("rock_wall_thickness") * mm;
  _rockRoofThickness = dp.getParameterAsDouble("rock_roof_thickness") * mm;
  _rockFloorThickness = dp.getParameterAsDouble("rock_floor_thickness") * mm;

  _concreteWallThickness = dp.getParameterAsDouble("concrete_wall_thickness") * mm;
  _concreteRoofThickness = dp.getParameterAsDouble("concrete_roof_thickness") * mm;
  _concreteFloorThickness = dp.getParameterAsDouble("concrete_floor_thickness") * mm;

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

  _isVisible = false;
  int iV = dp.getParameterAsInt("visible");
  if (iV == 1) {
    _isVisible = true;
  }
  G4cout << "PandaXLab found..." << G4endl;
}

G4bool PandaXLab::construct ()
{
  // add construction code here ...

  // the rock wall
  G4Material * marble = G4Material::GetMaterial("MARBLE");
  if (marble == 0) {
    G4cout << "Can't find the material MARBLE" << G4endl;
    return false;
  }

  G4Box * marbleBox = new G4Box("MarbleBox", _length/2.+_concreteWallThickness+_rockWallThickness, _width/2.+_concreteWallThickness+_rockWallThickness, _height/2. + (_concreteRoofThickness+_concreteFloorThickness)/2. + (_rockRoofThickness+_rockFloorThickness)/2.);
  _rockWallLogicalVolume = new G4LogicalVolume(marbleBox, marble, "MarbleBoxLog", 0, 0, 0);
  _partLogicalVolume = _rockWallLogicalVolume;
  _rockWallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5*(_height+_concreteRoofThickness+_rockRoofThickness-_concreteFloorThickness-_rockFloorThickness)), _partLogicalVolume, "RockWall", _parentPart->getContainerLogicalVolume(), false, 0);
  _partPhysicalVolume = _rockWallPhysicalVolume;

  // the concrete wall
  G4Material * concrete = G4Material::GetMaterial("G4_CONCRETE");
  G4Box * concreteBox = new G4Box("ConcreteBox", _length/2. + _concreteWallThickness, _width/2. + _concreteWallThickness, _height/2. + (_concreteRoofThickness+_concreteFloorThickness)/2.);
  _concreteWallLogicalVolume = new G4LogicalVolume(concreteBox, concrete, "ConcreteBoxLog", 0, 0, 0);
  _concreteWallPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5*(_rockFloorThickness-_rockRoofThickness)), _concreteWallLogicalVolume, "ConcreteWall", _rockWallLogicalVolume, false, 0);

  // the inner space
  G4Material * air = G4Material::GetMaterial("G4_AIR");
  G4Box * innerSpaceBox = new G4Box("InnerSpaceBox", _length/2., _width/2., _height/2.);
  _innerSpaceLogicalVolume = new G4LogicalVolume(innerSpaceBox, air, "InnerSpaceLog", 0, 0, 0);
  _innerSpacePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5*(_concreteFloorThickness-_concreteRoofThickness)), _innerSpaceLogicalVolume, "InnerSpaceWall", _concreteWallLogicalVolume, false, 0);

  _partContainerLogicalVolume = _innerSpaceLogicalVolume;
  return true;
}

