#include "detector/PandaXIIIWaterShield.hh"
#include "detector/BambooDetectorFactory.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "BambooGlobalVariables.hh"

#include <G4Material.hh>
#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4UnionSolid.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>

#include <G4VisAttributes.hh>

#include <math.h>

namespace {

  BambooDetectorPart * createPandaXIIIWaterShield (const G4String & name)
  {
    return new PandaXIIIWaterShield(name);
  }

  const std::string PandaXIIIWaterShieldName("PandaXIIIWaterShield");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXIIIWaterShieldName, createPandaXIIIWaterShield);
}

PandaXIIIWaterShield::PandaXIIIWaterShield (const G4String & name)
  : BambooDetectorPart(name)
{
  DetectorParameters dp = BambooGlobalVariables::Instance()
    ->findDetectorPartParameters(_partName);
  _radius = dp.getParameterAsDouble("radius") * mm;
  _length=dp.getParameterAsDouble("length")*mm;
  _height = dp.getParameterAsDouble("height") * mm;
  _shiftX = dp.getParameterAsDouble("shiftX") * mm;
  _shiftY = dp.getParameterAsDouble("shiftY") * mm;
  _shiftZ = dp.getParameterAsDouble("shiftZ") * mm;
  G4cout << "radius: " <<  _radius/mm << "mm." << G4endl;
}

G4bool PandaXIIIWaterShield::construct ()
{
  // water
  G4Material* Water=G4Material::GetMaterial("G4_WATER");

  //PandaXIIIConcreteShield is a boolean solid
  //emicylindrical extremity
  G4Tubs* Waterend=new G4Tubs("Waterend",0,_radius,.5*_height,-90*deg,180*deg);
  //box body
  G4Box* Waterbody=new G4Box("Waterbody",.5*_length,_radius,.5*_height);
  //2nd emicylindrical extremity needs to be flipped
  G4RotationMatrix* flip=new G4RotationMatrix(G4ThreeVector(0,0,1),180*deg);
  
  G4UnionSolid* PandaXIIIWaterShieldSol=new G4UnionSolid("preWater",Waterbody,Waterend,0,G4ThreeVector(.5*_length,0,0));
  PandaXIIIWaterShieldSol=new G4UnionSolid("PandaXIIIWaterShieldSol",PandaXIIIWaterShieldSol,
    Waterend,flip,G4ThreeVector(-.5*_length,0,0));
  _partLogicalVolume=new G4LogicalVolume(PandaXIIIWaterShieldSol,Water,"PandaXIIIWaterShieldLog", 0, 0, 0);
  _partPhysicalVolume=new G4PVPlacement(NULL,G4ThreeVector(_shiftX, _shiftY, _shiftZ),_partLogicalVolume,
    "PandaXIIIWaterShield", _parentPart->getContainerLogicalVolume(),false,0);
  _partContainerLogicalVolume = _partLogicalVolume;
  
  //visual attributes
  G4VisAttributes* PandaXIIIWaterShieldVisAtt=new G4VisAttributes(G4Colour(0. , 0. ,  1.,0.4));
  PandaXIIIWaterShieldVisAtt->SetForceSolid(1);
  _partLogicalVolume->SetVisAttributes(PandaXIIIWaterShieldVisAtt);
  
  return true;
}

