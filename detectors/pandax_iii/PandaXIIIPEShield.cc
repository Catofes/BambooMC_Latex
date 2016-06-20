#include "detector/PandaXIIIPEShield.hh"
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

  BambooDetectorPart * createPandaXIIIPEShield (const G4String & name)
  {
    return new PandaXIIIPEShield(name);
  }

  const std::string PandaXIIIPEShieldName("PandaXIIIPEShield");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXIIIPEShieldName, createPandaXIIIPEShield);
}

PandaXIIIPEShield::PandaXIIIPEShield (const G4String & name)
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

G4bool PandaXIIIPEShield::construct ()
{
  //G4NistManager* pNistManager = G4NistManager::Instance();
  
  // Polyethylene
  G4Material* Polyethylene=G4Material::GetMaterial("G4_POLYETHYLENE");

  //PandaXIIIConcreteShield is a boolean solid
  //emicylindrical extremity
  G4Tubs* PEend=new G4Tubs("PEend",0,_radius,.5*_height,-90*deg,180*deg);
  //box body
  G4Box* PEbody=new G4Box("PEbody",.5*_length,_radius,.5*_height);
  //2nd emicylindrical extremity needs to be flipped
  G4RotationMatrix* flipPE=new G4RotationMatrix(G4ThreeVector(0,0,1),180*deg);
  
  G4UnionSolid* PandaXIIIPEShieldSol=new G4UnionSolid("prePE",PEbody,PEend,0,G4ThreeVector(.5*_length,0,0));
  PandaXIIIPEShieldSol=new G4UnionSolid("PandaXIIIPEShieldSol",PandaXIIIPEShieldSol,
    PEend,flipPE,G4ThreeVector(-.5*_length,0,0));
  _partLogicalVolume=new G4LogicalVolume(PandaXIIIPEShieldSol,Polyethylene,"PandaXIIIPEShieldLog", 0, 0, 0);
  _partPhysicalVolume=new G4PVPlacement(NULL,G4ThreeVector(_shiftX, _shiftY, _shiftZ),_partLogicalVolume,
    "PandaXIIIPEShield", _parentPart->getContainerLogicalVolume(),false,0);
  _partContainerLogicalVolume = _partLogicalVolume;
  
  //visual attributes
  G4VisAttributes* PandaXIIIPEShieldVisAtt=new G4VisAttributes(G4Colour(178/255. , 178/255. ,  178/255.,0.3));
  PandaXIIIPEShieldVisAtt->SetForceSolid(1);
  _partLogicalVolume->SetVisAttributes(PandaXIIIPEShieldVisAtt);
  
  return true;
}

