#ifndef PANDAXIIIFAKEELECTRONICS_H
#define PANDAXIIIFAKEELECTRONICS_H

#include "detector/BambooDetectorPart.hh"

class PandaXIIIFakeElectronics : public BambooDetectorPart
{

public:

  PandaXIIIFakeElectronics(const G4String &name);

  virtual G4bool construct();

private:

  double _radius;
  double _distance;
};


#endif // PANDAXIIIFAKEELECTRONICS_H

