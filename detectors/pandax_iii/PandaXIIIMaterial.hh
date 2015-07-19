#ifndef PANDAXIIIMATERIAL_H
#define PANDAXIIIMATERIAL_H

#include "detector/BambooMaterial.hh"

class PandaXIIIMaterial : public BambooMaterial
{

public:

  PandaXIIIMaterial(const G4String &name);

  virtual void defineMaterials();

private:

};


#endif // PANDAXIIIMATERIAL_H
