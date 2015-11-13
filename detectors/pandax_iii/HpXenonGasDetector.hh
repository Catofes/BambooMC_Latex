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

  bool _isSensitive;

  G4LogicalVolume * _copperVesselLogicalVolume;
  G4VPhysicalVolume * _copperVesselPhysicalVolume;
  G4LogicalVolume * _XeTubLogicalVolume;
  G4VPhysicalVolume * _XeTubPhysicalVolume;
};


#endif // HPXENONGASDETECTOR_H

