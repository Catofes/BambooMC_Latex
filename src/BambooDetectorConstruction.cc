#include "BambooDetectorConstruction.hh"

#include <G4RunManager.hh>
#include <G4MaterialTable.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4GeometryManager.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4SolidStore.hh>

#include <G4Box.hh>
#include <G4PVPlacement.hh>

BambooDetectorConstruction::BambooDetectorConstruction () {
  _updated = false;
  _detectorMessenger = new BambooDetectorMessenger(this);
}

BambooDetectorConstruction::~BambooDetectorConstruction () {
}

G4VPhysicalVolume * BambooDetectorConstruction::Construct()
{
  DefineMaterials();
  return ConstructDetector();
}

void BambooDetectorConstruction::UpdateGeometry ()
{
  // clean-up previous geometry
  G4GeometryManager::GetInstance()->OpenGeometry();

  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetector());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  _updated = false;
}

void BambooDetectorConstruction::DefineMaterials ()
{
}

G4VPhysicalVolume * BambooDetectorConstruction::ConstructDetector ()
{
  constructLab();
  constructShield();
  return _worldPhysical;
}

void BambooDetectorConstruction::constructLab ()
{
  // construct world with marble.
  G4double worldHalfX = 6.0*m;
  G4double worldHalfY = 6.0*m;
  G4double worldHalfZ = 10.0*m;
  G4Material * marble = G4Material::GetMaterial("Marble");
  G4Box * worldBox = new G4Box("WorldBox", worldHalfX, worldHalfY, worldHalfZ);
  G4LogicalVolume * worldLog = new G4LogicalVolume(worldBox, marble, "WorldLog", 0, 0, 0);
  _worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLog, "World", 0, false, 0);
  worldLog->SetVisAttributes(G4VisAttributes::Invisible);
  // construct wall with concrete

  // construct lab with air
  G4double labHalfX = 3.25*m;
  G4double labHalfY = 3.75*m;
  G4double labHalfZ = 5.0*m;
  G4Material * air = G4Material::GetMaterial("G4_AIR");
  G4Box * labBox = new G4Box("labBox", labHalfX, labHalfY, labHalfZ);
  G4LogicalVolume * labLog = new G4LogicalVolume(labBox, air, "LabLog", 0, 0, 0);
  _labPhysical = new G4PVPlacement(0, G4ThreeVector(), labLog, "Lab", worldLog, false, 0);
}

void BambooDetectorConstruction::constructShield()
{
  G4LogicalVolume * labLog = _labPhysical->GetLogicalVolume();
  G4double posX = 2.0*m;
  G4double posY = 0.5*m;
  G4double posZ = 2.5*m;
}
