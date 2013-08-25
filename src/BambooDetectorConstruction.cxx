#include <G4NistManager.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4SystemOfUnits.hh>

#include "BambooDetectorConstruction.h"

BambooDetectorConstruction::BambooDetectorConstruction ()
  : G4VUserDetectorConstruction () {
}

BambooDetectorConstruction::~BambooDetectorConstruction () {
}

G4VPhysicalVolume * BambooDetectorConstruction::Construct () {
  G4VPhysicalVolume * room = buildWorldVolume();
  G4VPhysicalVolume * vessel = buildVessel(room);
  return room;
}

G4VPhysicalVolume * BambooDetectorConstruction::buildWorldVolume() {
  G4NistManager * nist = G4NistManager::Instance();

  G4Material * air = nist->FindOrBuildMaterial("G4_AIR");

  double x = 10.0*m;
  double y = 10.0*m;
  double z = 4.0*m;
  G4Box * worldBox = new G4Box("World", x, y, z);
  G4LogicalVolume* worldLog = new G4LogicalVolume(worldBox, air, "Room");
  
  G4VPhysicalVolume* roomPhys
    = new G4PVPlacement(0,
			G4ThreeVector(),
			worldLog,
			"World",
			0,
			false,
			0,
			true);
			
  return roomPhys;
}

G4VPhysicalVolume * BambooDetectorConstruction::buildVessel (G4VPhysicalVolume * container) {
  double innerRadius = 0.75*m;
  double outerRadius = 1.0*m;
  double height = 1.0*m;
  double angleStart = 0.*degree;
  double angleStop = 360.*degree;
  G4Tubs* vesselTub
    = new G4Tubs("VesselTub",
                 innerRadius,
                 outerRadius,
                 height,
                 angleStart,
                 angleStop);

  G4NistManager * nist = G4NistManager::Instance();
  G4Material * copper = nist->FindOrBuildMaterial("G4_Cu");
  
  G4LogicalVolume* vesselLog = new G4LogicalVolume(vesselTub, copper, "Vessel");

  G4VPhysicalVolume * vesselPhys
    = new G4PVPlacement(0,
			G4ThreeVector(),
			vesselLog,
			"Vessel",
			container->GetLogicalVolume(),
			false,
			0,
			true);
  return vesselPhys;
}
