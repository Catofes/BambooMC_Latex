#ifndef BAMBOODETECTORCONSTRUCTION_H
#define BAMBOODETECTORCONSTRUCTION_H

#include "BambooDetectorMessenger.hh"

#include <G4Material.hh>
#include <G4VisAttributes.hh>
#include <G4RotationMatrix.hh>
#include <G4VUserDetectorConstruction.hh>

class G4VPhysicalVolume;

class BambooDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  BambooDetectorConstruction();
  virtual ~BambooDetectorConstruction();

  virtual G4VPhysicalVolume * Construct();

  //rebuild the geometry based on changes
  void UpdateGeometry();
  G4bool GetUpdated() { return _updated; }

private:

  void DefineMaterials();

  G4VPhysicalVolume * ConstructDetector();

  void constructLab();
  void constructShield();

  // variables
  BambooDetectorMessenger * _detectorMessenger;

  G4bool _updated;

  // Physical Volume
  G4VPhysicalVolume * _worldPhysical;
  G4VPhysicalVolume * _wallPhysical;
  G4VPhysicalVolume * _labPhysical;
  // Materials & Elements

  // Geometry
};
#endif
