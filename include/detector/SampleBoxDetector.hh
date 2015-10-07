#ifndef SAMPLEBOXDETECTOR_H
#define SAMPLEBOXDETECTOR_H

#include "detector/BambooDetectorPart.hh"

class SampleBoxDetector : public BambooDetectorPart
{
public:
  SampleBoxDetector(const G4String &name);
  virtual G4bool construct();

private:
  double _halfX;
  double _halfY;
  double _halfZ;
  double _shiftX;
  double _shiftY;
  double _shiftZ;
  G4String _material;
};


#endif // SAMPLEBOXDETECTOR_H
