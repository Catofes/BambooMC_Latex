#include "BambooDetectorConstruction.hh"
#include "BambooGlobalVariables.hh"
#include "detector/BambooDetectorFactory.hh"

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
}

BambooDetectorConstruction::~BambooDetectorConstruction () {
}

G4VPhysicalVolume * BambooDetectorConstruction::Construct()
{
  //  DefineMaterials();
  //  return ConstructDetector();
  // read detectors
  BambooDetectorFactory * factory = BambooDetectorFactory::Instance();
  BambooGlobalVariables * bgv = BambooGlobalVariables::Instance();
  const vector<DetectorParameters> & dps = bgv->getDetectorParametersList();
  G4VPhysicalVolume * world = 0;
  for (size_t i=0; i<dps.size(); ++i) {
    const DetectorParameters & dp = dps[i];
    BambooDetectorPart * bdp = factory->createDetectorPart(dp.getDetectorPartName());
    if (!dp.getParentName().empty()) {
      BambooDetectorPart * parent = bgv->findDetectorPart(dp.getParentName());
      bdp->setParent(parent);
    }
    bgv->addDetectorPart(bdp);
    bdp->construct();
    if (dp.isWorldPart()) {
      world = bdp->getPhysicalVolume();
    }
  }
  return world;
}


void BambooDetectorConstruction::DefineMaterials ()
{
}

// void BambooDetectorConstruction::constructLab ()
// {
//   // construct world with marble.
//   G4double worldHalfX = 6.0*m;
//   G4double worldHalfY = 6.0*m;
//   G4double worldHalfZ = 10.0*m;
//   G4Material * marble = G4Material::GetMaterial("Marble");
//   G4Box * worldBox = new G4Box("WorldBox", worldHalfX, worldHalfY, worldHalfZ);
//   G4LogicalVolume * worldLog = new G4LogicalVolume(worldBox, marble, "WorldLog", 0, 0, 0);
//   _worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLog, "World", 0, false, 0);
//   worldLog->SetVisAttributes(G4VisAttributes::Invisible);
//   // construct wall with concrete

//   // construct lab with air
//   G4double labHalfX = 3.25*m;
//   G4double labHalfY = 3.75*m;
//   G4double labHalfZ = 5.0*m;
//   G4Material * air = G4Material::GetMaterial("G4_AIR");
//   G4Box * labBox = new G4Box("labBox", labHalfX, labHalfY, labHalfZ);
//   G4LogicalVolume * labLog = new G4LogicalVolume(labBox, air, "LabLog", 0, 0, 0);
//   _labPhysical = new G4PVPlacement(0, G4ThreeVector(), labLog, "Lab", worldLog, false, 0);
// }

// void BambooDetectorConstruction::constructShield()
// {
//   G4LogicalVolume * labLog = _labPhysical->GetLogicalVolume();
//   G4double posX = 2.0*m;
//   G4double posY = 0.5*m;
//   G4double posZ = 2.5*m;
// }
