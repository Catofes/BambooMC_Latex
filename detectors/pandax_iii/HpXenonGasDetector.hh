#ifndef HPXENONGASDETECTOR_H
#define HPXENONGASDETECTOR_H

#include "detector/BambooDetectorPart.hh"

class HpXenonGasDetector : public BambooDetectorPart
{

public:

  HpXenonGasDetector(const G4String &name);

  virtual G4bool construct();

private:

  double _vesselOuterRadius;
  double _vesselBarrelThickness;
  double _vesselEndThickness;

  double _vesselHeight;
  double _shiftX;
  double _shiftY;
  double _shiftZ;

  double _xenonPressure;
  double _xenonTemperature;

  G4LogicalVolume * _copperVesselLogicalVolume;
  G4VPhysicalVolume * _copperVesselPhysicalVolume;
  G4LogicalVolume * _hpXenonLogicalVolume;
  G4VPhysicalVolume * _hpXenonPhysicalVolume;
};


#endif // HPXENONGASDETECTOR_H

