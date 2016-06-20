#include "detector/PandaXIIIConcreteShield.hh"
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

// anonymous namespace to register the SampleCylinderDetector

namespace {

  BambooDetectorPart * createPandaXIIIConcreteShield (const G4String & name)
  {
    return new PandaXIIIConcreteShield(name);
  }

  const std::string PandaXIIIConcreteShieldType("PandaXIIIConcreteShield");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXIIIConcreteShieldType, createPandaXIIIConcreteShield);
}

PandaXIIIConcreteShield::PandaXIIIConcreteShield (const G4String & name)
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

G4bool PandaXIIIConcreteShield::construct ()
{
  // Concrete
  G4Material* Concrete=G4Material::GetMaterial("Sigconcrete");

  //PandaXIIIConcreteShield is a boolean solid
  //emicylindrical extremity
  G4Tubs* concend=new G4Tubs("concend",0,_radius,.5*_height,-90*deg,180*deg);
  //box body
  G4Box* concbody=new G4Box("concbody",.5*_length,_radius,.5*_height);//G4LogicalVolume* logconcbody=new G4LogicalVolume(concbody,Concrete,"logconcbody");
  //2nd emicylindrical extremity needs to be flipped
  G4RotationMatrix* flipconc=new G4RotationMatrix(G4ThreeVector(0,0,1),180*deg);
  
  G4UnionSolid* PandaXIIIConcreteShieldSol=new G4UnionSolid("preconc",concbody,concend,0,G4ThreeVector(.5*_length,0,0));
  PandaXIIIConcreteShieldSol=new G4UnionSolid("PandaXIIIConcreteShieldSol",PandaXIIIConcreteShieldSol,
    concend,flipconc,G4ThreeVector(-.5*_length,0,0));
  _partLogicalVolume=new G4LogicalVolume(PandaXIIIConcreteShieldSol,Concrete,"PandaXIIIConcreteShieldLog", 0, 0, 0);
  _partPhysicalVolume=new G4PVPlacement(NULL,G4ThreeVector(_shiftX, _shiftY, _shiftZ),_partLogicalVolume,
    "PandaXIIIConcreteShield", _parentPart->getContainerLogicalVolume(),false,0);
  _partContainerLogicalVolume = _partLogicalVolume;
  
  //visual attributes
  G4VisAttributes* PandaXIIIConcreteShieldVisAtt=new G4VisAttributes(G4Colour(0.2,0.2,0.2,0.3));
  PandaXIIIConcreteShieldVisAtt->SetForceSolid(1);
  _partLogicalVolume->SetVisAttributes(PandaXIIIConcreteShieldVisAtt);
  
  return true;
}

