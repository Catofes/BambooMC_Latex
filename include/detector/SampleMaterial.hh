#ifndef SAMPLEMATERIAL_H
#define SAMPLEMATERIAL_H

#include "detector/BambooMaterial.hh"

class SampleMaterial : public BambooMaterial
{

public:

  SampleMaterial(const G4String &name);

  virtual void defineMaterials();

private:

};


#endif // SAMPLEMATERIAL_H

