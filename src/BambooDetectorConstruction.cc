#include "BambooDetectorConstruction.hh"
#include "BambooGlobalVariables.hh"
#include "detector/BambooDetectorFactory.hh"
#include "detector/BambooMaterialFactory.hh"

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

BambooDetectorConstruction::BambooDetectorConstruction()
{
}

BambooDetectorConstruction::~BambooDetectorConstruction()
{
}

G4VPhysicalVolume *BambooDetectorConstruction::Construct()
{
    // define materials
    DefineMaterials();

    // read detectors
    BambooDetectorFactory *factory = BambooDetectorFactory::Instance();
    BambooGlobalVariables *bgv = BambooGlobalVariables::Instance();
    const vector<DetectorParameters> &dps = bgv->getDetectorParametersList();
    G4VPhysicalVolume *world = 0;
    for (size_t i = 0; i < dps.size(); ++i) {
        const DetectorParameters &dp = dps[i];
        BambooDetectorPart *bdp = factory->createDetectorPart(dp.getDetectorPartType(), dp.getDetectorPartName());
        if (!dp.getParentName().empty()) {
            BambooDetectorPart *parent = bgv->findDetectorPart(dp.getParentName());
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


void BambooDetectorConstruction::DefineMaterials()
{
    BambooMaterialFactory *matFactory = BambooMaterialFactory::Instance();
    BambooGlobalVariables *bgv = BambooGlobalVariables::Instance();
    if (bgv->getMaterialName() != "") {
        BambooMaterial *material = matFactory->createMaterial(bgv->getMaterialName());
        material->defineMaterials();
    }
}
