#ifndef HPXENONGASDETECTOR_H
#define HPXENONGASDETECTOR_H

#include "detector/BambooDetectorPart.hh"

class G4Material;

class HpXenonGasDetector : public BambooDetectorPart
{

public:

  HpXenonGasDetector(const G4String &name);

  virtual G4bool construct();

private:

  void createEnrichedXenon();

  void createXenonVolume(bool top);

  double _vesselOuterRadius;
  double _vesselBarrelThickness;
  double _vesselEndThickness;

  double _vesselHeight;
  double _shiftX;
  double _shiftY;
  double _shiftZ;

  double _xenonPressure;
  double _xenonTemperature;

  double _electricFieldZ;

  double _xe136Fraction;

  G4Material * _hpXe;
  G4LogicalVolume * _copperVesselLogicalVolume;
  G4VPhysicalVolume * _copperVesselPhysicalVolume;
  G4LogicalVolume * _hpXenonLogicalVolumeTop;
  G4LogicalVolume * _hpXenonLogicalVolumeBottom;
  G4VPhysicalVolume * _hpXenonPhysicalVolumeTop;
  G4VPhysicalVolume * _hpXenonPhysicalVolumeBottom;
};


#endif // HPXENONGASDETECTOR_H

