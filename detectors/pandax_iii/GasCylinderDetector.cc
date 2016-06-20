#include "detector/GasCylinderDetector.hh"
#include "detector/BambooDetectorFactory.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4Tubs.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>

#include <G4VisAttributes.hh>

#include <math.h>

// anonymous namespace to register the GasCylinderDetector

namespace
{

    BambooDetectorPart *createGasCylinderDetector(const G4String &name)
    {
        return new GasCylinderDetector(name);
    }

    const std::string GasCylinderDetectorType("GasCylinderDetector");

    const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(GasCylinderDetectorType,
                                                                                    createGasCylinderDetector);
}

GasCylinderDetector::GasCylinderDetector(const G4String &name)
        : BambooDetectorPart(name)
{
    DetectorParameters dp = BambooGlobalVariables::Instance()
            ->findDetectorPartParameters(_partName);
    _radius = dp.getParameterAsDouble("radius") * mm;
    _height = dp.getParameterAsDouble("height") * mm;
    _shiftX = dp.getParameterAsDouble("shiftX") * mm;
    _shiftY = dp.getParameterAsDouble("shiftY") * mm;
    _shiftZ = dp.getParameterAsDouble("shiftZ") * mm;
    G4cout << "radius: " << _radius / mm << "mm." << G4endl;
}

G4bool GasCylinderDetector::construct()
{
    //enriched Xe at 10 bar
    G4Material *xenon = G4Material::GetMaterial("HP_Xe_enriched");

    G4Tubs *GasCylinderDetectorSolid = new G4Tubs("GasCylinderDetectorSolid", 0, _radius, _height / 2., 0., 2. * M_PI);
    _partLogicalVolume = new G4LogicalVolume(GasCylinderDetectorSolid, xenon, "GasCylinderDetectorLog", 0, 0, 0);
    _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ), _partLogicalVolume,
                                            "GasCylinderDetector", _parentPart->getContainerLogicalVolume(), false, 0);
    _partContainerLogicalVolume = _partLogicalVolume;
    G4VisAttributes *sampleDetectorVisAtt = new G4VisAttributes(G4Colour(0., 0.6, 0.1, .4));
    sampleDetectorVisAtt->SetForceSolid(1);
    _partLogicalVolume->SetVisAttributes(sampleDetectorVisAtt);
    PandaXSensitiveDetector *cylinderSD = new PandaXSensitiveDetector("GasCylinderSD");
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(cylinderSD);
    _partLogicalVolume->SetSensitiveDetector(cylinderSD);
    G4cout << "Sensitive Mass: " << _partLogicalVolume->GetMass() / kg << " kg." << G4endl;
    return true;
}

