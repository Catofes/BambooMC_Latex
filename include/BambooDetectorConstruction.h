#ifndef BAMBOODETECTORCONSTRUCTION_H
#define BAMBOODETECTORCONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>

class BambooDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  BambooDetectorConstruction ();
  virtual ~BambooDetectorConstruction();

  virtual G4VPhysicalVolume * Construct();
};

#endif // BAMBOODETECTORCONSTRUCTION_H
