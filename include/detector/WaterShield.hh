#ifndef WATERSHIELD_H
#define WATERSHIELD_H

#include "detector/BambooDetectorPart.hh"

class WaterShield : public BambooDetectorPart
{

public:

  WaterShield(const G4String &name);

  virtual G4bool construct();

private:

  int _shape;			//! 1: cubic,  2: cylinder 
  double _length;
  double _width;
  double _height;

  double _radius;
};


#endif // WATERSHIELD_H

