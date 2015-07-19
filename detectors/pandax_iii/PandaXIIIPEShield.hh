#ifndef PDXIIIPESHIELD_H
#define PDXIIIPESHIELD_H 1

#include "detector/BambooDetectorPart.hh"

class PandaXIIIPEShield : public BambooDetectorPart
{
public:
  PandaXIIIPEShield(const G4String &name);
  virtual G4bool construct();

private:
  double _radius;
  double _length;
  double _height;
  double _shiftX;
  double _shiftY;
  double _shiftZ;
};


#endif // PDXIIIPESHIELD_H
