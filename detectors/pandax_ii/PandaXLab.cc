#include "detector/PandaXLab.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

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
   
  G4cout << "PandaXLab found..." << G4endl;
}

G4bool PandaXLab::construct ()
{
  // add construction code here ...
  
  return true;
}

