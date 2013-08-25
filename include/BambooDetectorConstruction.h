#ifndef BAMBOODETECTORCONSTRUCTION_H
#define BAMBOODETECTORCONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

class BambooDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  BambooDetectorConstruction ();
  virtual ~BambooDetectorConstruction();

  virtual G4VPhysicalVolume * Construct();

private:
  G4VPhysicalVolume * buildWorldVolume ();
  G4VPhysicalVolume * buildVessel (G4VPhysicalVolume * container);
};

#endif // BAMBOODETECTORCONSTRUCTION_H
