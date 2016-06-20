#include "detector/PandaXIIIOuterRock.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <vector>

// anonymous namespace to register the SampleWorldBox

namespace {

  BambooDetectorPart * createPandaXIIIOuterRock (const G4String & name)
  {
    return new PandaXIIIOuterRock(name);
  }

  const std::string PandaXIIIOuterRockName("PandaXIIIOuterRock");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXIIIOuterRockName, createPandaXIIIOuterRock);
}

PandaXIIIOuterRock::PandaXIIIOuterRock (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);
  _halfX = dp.getParameterAsDouble("half_x") * mm;
  _halfY = dp.getParameterAsDouble("half_y") * mm;
  _halfZ = dp.getParameterAsDouble("half_z") * mm;
  _shiftX = dp.getParameterAsDouble("shiftX") * mm;
  _shiftY = dp.getParameterAsDouble("shiftY") * mm;
  _shiftZ = dp.getParameterAsDouble("shiftZ") * mm;
  G4cout << "PandaXIIIOuterRock found..." <<"with _halfX "<<_halfX<<", _halfY "<<_halfY<<", _halfZ "<<_halfZ<< G4endl;
}

G4bool PandaXIIIOuterRock::construct ()
{
  G4Material* Marble=G4Material::GetMaterial("MARBLE");
  
  G4Box * PandaXIIIOuterRockBox = new G4Box("PandaXIIIOuterRockBox", _halfX, _halfY, _halfZ);
  _partLogicalVolume = new G4LogicalVolume(PandaXIIIOuterRockBox, Marble, "PandaXIIIOuterRockLog", 0, 0, 0);
  _partPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(), _partLogicalVolume, "PandaXIIIOuterRock", _parentPart->getContainerLogicalVolume(), false, 0);
  _partContainerLogicalVolume = _partLogicalVolume;
  G4VisAttributes * PandaXIIIOuterRockVis = new G4VisAttributes(G4Colour(0.,1,0.,0.25));
  PandaXIIIOuterRockVis->SetForceSolid(1);
  _partLogicalVolume->SetVisAttributes(PandaXIIIOuterRockVis);
  
  G4cout<<_partPhysicalVolume->GetName()<<" is built ..."<<G4endl;
  
  return true;
}

