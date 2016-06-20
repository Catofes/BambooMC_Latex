#include "detector/CopperVessel.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Tubs.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>

// anonymous namespace to register the CopperVessel

namespace {

  BambooDetectorPart * createCopperVessel (const G4String & name)
  {
    return new CopperVessel(name);
  }

  const std::string CopperVesselName("CopperVessel");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(CopperVesselName, createCopperVessel);
}

CopperVessel::CopperVessel (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);

  _outerRadius = dp.getParameterAsDouble("outerRadius") * mm;
  _wallThickness = dp.getParameterAsDouble("wallThickness") * mm;
  _topThickness = dp.getParameterAsDouble("topThickness") * mm;
  _height = dp.getParameterAsDouble("height") * mm;
  _shiftX = dp.getParameterAsDouble("shiftX") * mm;
  _shiftY = dp.getParameterAsDouble("shiftY") * mm;
  _shiftZ = dp.getParameterAsDouble("shiftZ") * mm;
  G4cout << "CopperVessel found..." << G4endl;
}

G4bool CopperVessel::construct ()
{
  G4Material * copper = G4Material::GetMaterial("G4_Cu");

  G4VSolid * vesselTub = new G4Tubs("CopperVesselTub", 0, _outerRadius, _height/2., 0., 2.*pi);

  G4LogicalVolume * vesselLog = new G4LogicalVolume(vesselTub, copper, "CopperVesselLog", 0, 0, 0);
  G4VPhysicalVolume * vesselPhy = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ), vesselLog, "CopperVessel", _parentPart->getContainerLogicalVolume(), false, 0);

  _partLogicalVolume = vesselLog;
  _partPhysicalVolume = vesselPhy;
  G4VisAttributes * copperVesselVisAtt = new G4VisAttributes(G4Colour(207./255, 180./255, 132./255));
  vesselLog->SetVisAttributes(copperVesselVisAtt);

  G4Material * vacuum = G4Material::GetMaterial("vacuum");
  G4VSolid * vacuumTub = new G4Tubs("CopperVesselVacuumTub", 0, _outerRadius-_wallThickness, _height/2.-_topThickness, 0., 2.*pi);

  G4LogicalVolume * vacuumLog = new G4LogicalVolume(vacuumTub, vacuum, "CopperVesselVacuumLog", 0, 0, 0);
  G4VPhysicalVolume * vacuumPhy = new G4PVPlacement(0, G4ThreeVector(), vacuumLog, "CopperVesselVacuumPhy", vesselLog, false, 0);
  _partContainerLogicalVolume = vacuumLog;

  G4cout << vacuumPhy->GetName() << " is built ..." << G4endl;
  return true;
}

