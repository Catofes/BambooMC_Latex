#ifndef PANDAXSIMPLEWATERSHIELD_H
#define PANDAXSIMPLEWATERSHIELD_H

#include "detector/BambooDetectorPart.hh"

class PandaXSimpleWaterShield : public BambooDetectorPart
{

public:

  PandaXSimpleWaterShield(const G4String &name);

  virtual G4bool construct();

private:

  double _length;
  double _width;
  double _height;

  bool _isVisible;
  bool _countingFluxIn;
};


#endif // PANDAXSIMPLEWATERSHIELD_H

