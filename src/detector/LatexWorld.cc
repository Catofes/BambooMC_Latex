#include "detector/LatexWorld.hh"
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

    BambooDetectorPart *createLatexWorld(const G4String &name)
    {
        return new LatexWorld(name);
    }

    const std::string LatexWorldName("LatexWorld");

    const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(LatexWorldName, createLatexWorld);
}

LatexWorld::LatexWorld(const G4String &name)
        : BambooDetectorPart(name)
{
    DetectorParameters dp = BambooGlobalVariables::Instance()
            ->findDetectorPartParameters(_partName);
    _halfX = BambooUtils::evaluate(dp.getParameterAsString("half_x"));
    _halfY = BambooUtils::evaluate(dp.getParameterAsString("half_y"));
    _halfZ = BambooUtils::evaluate(dp.getParameterAsString("half_z"));
    _magnetic_field_B = dp.getParameterAsDouble("B");
    _magnetic_field = dp.getParameterAsInt("magnet") == 1 ? true : false;
    if (_halfX <= 0)
        _halfX = 1 * m;
    if (_halfY <= 0)
        _halfY = 1 * m;
    if (_halfZ <= 0)
        _halfZ = 1 * m;
}

G4bool LatexWorld::construct()
{
    G4Material *air = G4Material::GetMaterial("G4_AIR");
    G4Box *worldBox = new G4Box("WorldBox", _halfX, _halfY, _halfZ);
    _partLogicalVolume = new G4LogicalVolume(worldBox, air, "WorldLog", 0, 0, 0);
    _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "World", 0, false, 0);
    _partContainerLogicalVolume = _partLogicalVolume;

    if (_magnetic_field) {
        G4UniformMagField *magField = new G4UniformMagField(G4ThreeVector(0, 0, _magnetic_field_B));
        G4FieldManager *fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
        fieldMgr->SetDetectorField(magField);
        fieldMgr->CreateChordFinder(magField);
    }

    return true;
}