#ifndef GASCYLINDERDETECTOR_H
#define GASCYLINDERDETECTOR_H

#include "detector/BambooDetectorPart.hh"

class GasCylinderDetector : public BambooDetectorPart
{
public:
  GasCylinderDetector(const G4String &name);
  virtual G4bool construct();

private:
  double _radius;
  double _height;
  double _shiftX;
  double _shiftY;
  double _shiftZ;
};


#endif // SAMPLECYLINDERDETECTOR_H
