//
// Created by herbertqiao on 1/13/16.
//
#include "detector/PandaXIIIVesselSupporter.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "BambooUtils.hh"

#include <G4Isotope.hh>
#include <G4Element.hh>
#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4Tubs.hh>
#include <G4SDManager.hh>
#include <G4UniformElectricField.hh>
#include <G4FieldManager.hh>
#include <G4EqMagElectricField.hh>
#include <G4ClassicalRK4.hh>
#include <G4MagIntegratorStepper.hh>
#include <G4MagIntegratorDriver.hh>
#include <G4ChordFinder.hh>

// anonymous namespace to register the PandaXIIIVesselSupporter

namespace {

    BambooDetectorPart *createPandaXIIIVesselSupporter(const G4String & name) {
        return new PandaXIIIVesselSupporter(name);
    }

    const std::string PandaXIIIVesselSupporterName("PandaXIIIVesselSupporter");

    const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXIIIVesselSupporterName,
                                                                                    createPandaXIIIVesselSupporter);
}

PandaXIIIVesselSupporter::PandaXIIIVesselSupporter(const G4String &name)
        : BambooDetectorPart(name) {

    DetectorParameters dp = BambooGlobalVariables::Instance()
            ->findDetectorPartParameters(_partName);

    _steelShellInnerRadius = BambooUtils::evaluate(dp.getParameterAsString("shell_inner_radius"));
    _steelShellBarrelThickness = BambooUtils::evaluate(dp.getParameterAsString("shell_barrel_thickness"));
    _steelShellEndThickness = BambooUtils::evaluate(dp.getParameterAsString("shell_end_thickness"));
    _steelShellInnerHeight = BambooUtils::evaluate(dp.getParameterAsString("shell_inner_height"));

    _shiftX = BambooUtils::evaluate(dp.getParameterAsString("shift_x"));
    _shiftY = BambooUtils::evaluate(dp.getParameterAsString("shift_y"));
    _shiftZ = BambooUtils::evaluate(dp.getParameterAsString("shift_z"));

    if (_steelShellInnerRadius <= 0) {
        _steelShellInnerRadius = 3000 * mm;
    }
    if (_steelShellInnerHeight <= 0) {
        _steelShellInnerHeight = 10000. * mm;
    }
    if (_steelShellBarrelThickness <= 0) {
        _steelShellBarrelThickness = 25. * mm;
    }
    if (_steelShellEndThickness <= 0) {
        _steelShellEndThickness = 25. * mm;
    }
    if (_shiftX == 0) {
        _shiftX = 0 * mm;
    }
    if (_shiftY == 0) {
        _shiftY = 0 * mm;
    }
    if (_shiftZ == 0) {
        _shiftZ = 0 * mm;
    }
    G4cout << "PandaXIIIVesselSupporter found..." << G4endl;
}

G4bool PandaXIIIVesselSupporter::construct() {
    G4Material *steel = G4Material::GetMaterial("G4_STAINLESS-STEEL");
    G4VSolid *steelShellTub = new G4Tubs("SteelShellTub", 0, _steelShellInnerRadius + _steelShellBarrelThickness,
                                         _steelShellInnerHeight / 2. + _steelShellEndThickness, 0., 2. * pi);
    _steelShellLogicalVolume = new G4LogicalVolume(steelShellTub, steel, "SteelShellTubLog", 0, 0, 0);

    G4RotationMatrix *mat = new G4RotationMatrix();
    mat->rotateY(pi / 2);
    _steelShellPhysicalVolume = new G4PVPlacement(mat, G4ThreeVector(_shiftX, _shiftY, _shiftZ),
                                                  _steelShellLogicalVolume, "SteelShell",
                                                  _parentPart->getContainerLogicalVolume(), false, 0);

    G4VisAttributes *vesselVis = new G4VisAttributes();
    vesselVis->SetColour(230. / 255, 230. / 255, 230. / 255, 0.4);
    _steelShellLogicalVolume->SetVisAttributes(vesselVis);


    G4Material *water = G4Material::GetMaterial("G4_WATER");
    G4VSolid *waterTub = new G4Tubs("waterTub", 0, _steelShellInnerRadius, _steelShellInnerHeight / 2., 0, 2 * pi);
    _waterTubLogicalVolume = new G4LogicalVolume(waterTub, water, "waterTubLog", 0, 0, 0);
    _waterTubPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _waterTubLogicalVolume, "WaterTub",
                                                _steelShellLogicalVolume, false, 0);

    vesselVis = new G4VisAttributes();
    vesselVis->SetColour(30. / 204, 0. / 255, 185. / 255, 0.4);
    _waterTubLogicalVolume->SetVisAttributes(vesselVis);

    _partLogicalVolume = _steelShellLogicalVolume;
    _partPhysicalVolume = _steelShellPhysicalVolume;
    _partContainerLogicalVolume = _waterTubLogicalVolume;

    return true;
}

