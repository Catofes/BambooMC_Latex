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

  void createXenonVolume(bool top);

  double _vesselInnerRadius;
  double _vesselBarrelThickness;
  double _vesselEndThickness;

  double _vesselInnerHeight;
  double _shiftX;
  double _shiftY;
  double _shiftZ;

  double _electricFieldZ;

  G4LogicalVolume * _copperVesselLogicalVolume;
  G4VPhysicalVolume * _copperVesselPhysicalVolume;
  G4LogicalVolume * _hpXenonLogicalVolumeTop;
  G4LogicalVolume * _hpXenonLogicalVolumeBottom;
  G4VPhysicalVolume * _hpXenonPhysicalVolumeTop;
  G4VPhysicalVolume * _hpXenonPhysicalVolumeBottom;
};


#endif // HPXENONGASDETECTOR_H

