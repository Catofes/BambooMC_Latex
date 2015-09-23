#ifndef PANDAXIIIMATERIAL_H
#define PANDAXIIIMATERIAL_H

#include "detector/BambooMaterial.hh"

class PandaXIIIMaterial : public BambooMaterial
{

public:

  PandaXIIIMaterial(const G4String &name);

  virtual void defineMaterials();

private:

  double _xe136Fraction;
  double _xenonTemperature;
  double _xenonPressure;
  double _enrichedXenonDensity;
  double _tmaMassFraction;
};


#endif // PANDAXIIIMATERIAL_H
