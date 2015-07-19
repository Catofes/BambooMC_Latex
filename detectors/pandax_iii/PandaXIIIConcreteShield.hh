#ifndef PDXIIICONCRETESHIELD_H
#define PDXIIICONCRETESHIELD_H 1

#include "detector/BambooDetectorPart.hh"

class PandaXIIIConcreteShield : public BambooDetectorPart
{
public:
  PandaXIIIConcreteShield(const G4String &name);
  virtual G4bool construct();

private:
  double _radius;
  double _length;
  double _height;
  double _shiftX;
  double _shiftY;
  double _shiftZ;
};


#endif // PDXIIICONCRETESHIELD_H
