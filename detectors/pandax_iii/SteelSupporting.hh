#ifndef STEELSUPPORTING_H
#define STEELSUPPORTING_H 1

#include "detector/BambooDetectorPart.hh"

class SteelSupporting : public BambooDetectorPart
{
public:
  SteelSupporting(const G4String &name);
  virtual G4bool construct();

private:
  double _shiftX;
  double _shiftY;
  double _shiftZ;
};


#endif // STEELSUPPORTING
