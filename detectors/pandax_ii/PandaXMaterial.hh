#ifndef PANDAXMATERIAL_H
#define PANDAXMATERIAL_H

#include "detector/BambooMaterial.hh"

class PandaXMaterial : public BambooMaterial
{

public:

  PandaXMaterial(const G4String &name);

  virtual void defineMaterials();

private:

};


#endif // PANDAXMATERIAL_H

