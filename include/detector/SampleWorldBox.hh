#ifndef SAMPLEWORLDBOX_H
#define SAMPLEWORLDBOX_H

#include "detector/BambooDetectorPart.hh"

class SampleWorldBox : public BambooDetectorPart
{
public:
  SampleWorldBox(const G4String &name);
  virtual G4bool construct();

private:
  double _halfX;
  double _halfY;
  double _halfZ;
};


#endif // SAMPLEWORLDBOX_H
