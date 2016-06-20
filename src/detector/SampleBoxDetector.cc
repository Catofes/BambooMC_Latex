#include "detector/SampleBoxDetector.hh"
#include "detector/BambooDetectorFactory.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>
#include <G4VisAttributes.hh>
// anonymous namespace to register the SampleBoxDetector

namespace {

    BambooDetectorPart *createSampleBoxDetector(const G4String & name) {
        return new SampleBoxDetector(name);
    }

    const std::string SampleBoxDetectorName("SampleBoxDetector");

    const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(SampleBoxDetectorName,
                                                                                    createSampleBoxDetector);
}

SampleBoxDetector::SampleBoxDetector(const G4String &name)
        : BambooDetectorPart(name) {
    DetectorParameters dp = BambooGlobalVariables::Instance()
            ->findDetectorPartParameters(_partName);
    _halfX = BambooUtils::evaluate(dp.getParameterAsString("half_x"));
    _halfY = BambooUtils::evaluate(dp.getParameterAsString("half_y"));
    _halfZ = BambooUtils::evaluate(dp.getParameterAsString("half_z"));
    _shiftX = BambooUtils::evaluate(dp.getParameterAsString("shift_x"));
    _shiftY = BambooUtils::evaluate(dp.getParameterAsString("shift_y"));
    _shiftZ = BambooUtils::evaluate(dp.getParameterAsString("shift_z"));
    _material = dp.getParameterAsString("material");
    if (_halfX == 0) {
        _halfX = 1.0 * m;
    }
    if (_halfY == 0) {
        _halfY = 1.0 * m;
    }
    if (_halfZ == 0) {
        _halfZ = 1.0 * m;
    }
    if (_shiftX == 0) {
        _shiftX = 0.0 * m;
    }
    if (_shiftY == 0) {
        _shiftY = 0.0 * m;
    }
    if (_shiftZ == 0) {
        _shiftZ = 0.0 * m;
    }
    if (_material == ""){
        _material = "G4_AIR";
    }
    G4cout << "SampleBoxDetector found..." << G4endl;
}

G4bool SampleBoxDetector::construct() {
    G4Material *liquidXenon = G4Material::GetMaterial(_material);
    G4Box *sampleDetectorBox = new G4Box("SampleDetectorBox", _halfX, _halfY, _halfZ);
    _partLogicalVolume = new G4LogicalVolume(sampleDetectorBox, liquidXenon, "SampleDetectorLog", 0, 0, 0);
    _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ), _partLogicalVolume,
                                            "SampleDetector", _parentPart->getContainerLogicalVolume(), false, 0);
    _partContainerLogicalVolume = _partLogicalVolume;

    // visibility attributes
    G4VisAttributes *sampleBoxVisAtt = new G4VisAttributes(G4Colour(0., 0.6, 0.2));
    _partLogicalVolume->SetVisAttributes(sampleBoxVisAtt);

    // add sensitive detector for energy deposition
    PandaXSensitiveDetector *sampleBoxSD = new PandaXSensitiveDetector("SampleBoxSD");
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(sampleBoxSD);
    _partLogicalVolume->SetSensitiveDetector(sampleBoxSD);

    return true;
}

