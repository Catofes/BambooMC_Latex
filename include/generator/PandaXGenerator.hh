#ifndef PANDAXGENERATOR_H
#define PANDAXGENERATOR_H

#include "generator/BambooGenerator.hh"

class G4GeneralParticleSource;
class G4Event;

class PandaXGenerator : public BambooGenerator {
public:

  PandaXGenerator(const G4String &name);

  ~PandaXGenerator();

  virtual void GeneratePrimaries(G4Event * anEvent);

private:

  G4GeneralParticleSource * _particleGun;

};
#endif //PANDAXGENERATOR_H
