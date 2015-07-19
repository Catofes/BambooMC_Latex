#ifndef PDXIIIOUTERROCK_H
#define PDXIIIOUTERROCK_H

#include "detector/BambooDetectorPart.hh"

class PandaXIIIOuterRock : public BambooDetectorPart
{
public:
  PandaXIIIOuterRock(const G4String &name);
  virtual G4bool construct();

private:
  double _halfX;
  double _halfY;
  double _halfZ;
  double _shiftX;
  double _shiftY;
  double _shiftZ;
};


#endif // PDXIIIOUTERROCK_H
