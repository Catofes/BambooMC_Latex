#include "detector/LatexLayer.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Field.hh>
#include <G4UniformMagField.hh>
#include <G4TransportationManager.hh>
#include <G4FieldManager.hh>

namespace
{

    BambooDetectorPart *createLatexLayer(const G4String &name)
    {
        return new LatexLayer(name);
    }

    const std::string LatexLayerName("LatexLayer");

    const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(LatexLayerName, createLatexLayer);
}

LatexLayer::LatexLayer(const G4String &name)
        : BambooDetectorPart(name)
{
    DetectorParameters dp = BambooGlobalVariables::Instance()
            ->findDetectorPartParameters(_partName);
    _halfX = BambooUtils::evaluate(dp.getParameterAsString("half_x"));
    _halfY = BambooUtils::evaluate(dp.getParameterAsString("half_y"));
    _latexZ = BambooUtils::evaluate(dp.getParameterAsString("latex_z"));
    _PETZ = BambooUtils::evaluate(dp.getParameterAsString("PET_z"));
    _shiftX = BambooUtils::evaluate(dp.getParameterAsString("shift_x"));
    _shiftY = BambooUtils::evaluate(dp.getParameterAsString("shift_y"));
    _shiftZ = BambooUtils::evaluate(dp.getParameterAsString("shift_z"));
    if (_halfX <= 0)
        _halfX = 10 * cm;
    if (_halfY <= 0)
        _halfY = 10 * cm;
    if (_latexZ <= 0)
        _latexZ = 100 * um;
    if (_PETZ <= 0)
        _PETZ = 200 * um;
}

G4bool LatexLayer::construct()
{
    G4Material *pet = G4Material::GetMaterial("G4_TEFLON");
    G4Material *latex = G4Material::GetMaterial("LATEX");

    G4Box *latexBox = new G4Box("latexBox", _halfX, _halfY, _latexZ / 2.);
    G4LogicalVolume *latexLog = new G4LogicalVolume(latexBox, latex, "latexLog", 0, 0, 0);

    G4Box *petBox = new G4Box("petBox", _halfX, _halfY, _latexZ / 2.);
    G4LogicalVolume *petLog = new G4LogicalVolume(petBox, pet, "petLog", 0, 0, 0);

    _partLogicalVolume = petLog;

    G4String petName = _partName;
    petName.append(G4String("PET"));
    _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ), petLog, petName,
                                            _parentPart->getContainerLogicalVolume(), false, 0);
    _partContainerLogicalVolume = NULL;

    G4String upLatexName = _partName;
    upLatexName.append(G4String("UpLatex"));
    _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ + _latexZ / 2. + _PETZ / 2.),
                                            latexLog, upLatexName, _parentPart->getContainerLogicalVolume(), false, 0);
    G4String downLatexName = _partName;
    downLatexName.append(G4String("DownLatex"));
    _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(_shiftX, _shiftY, _shiftZ - _latexZ / 2. - _PETZ / 2.),
                                            latexLog, downLatexName, _parentPart->getContainerLogicalVolume(), false,
                                            0);

    return true;
}