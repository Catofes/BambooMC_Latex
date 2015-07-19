#ifndef PDXIIIWATERSHIELD_H
#define PDXIIIWATERSHIELD_H 1

#include "detector/BambooDetectorPart.hh"

class PandaXIIIWaterShield : public BambooDetectorPart
{
public:
  PandaXIIIWaterShield(const G4String &name);
  virtual G4bool construct();

private:
  double _radius;
  double _length;
  double _height;
  double _shiftX;
  double _shiftY;
  double _shiftZ;
};


#endif // PDXIIIWATERSHIELD_H
