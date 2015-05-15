#ifndef PANDAXGENERATOR_H
#define PANDAXGENERATOR_H

#include "generator/BambooGenerator.hh"
#include "generator/PandaXParticleSource.hh"

class G4GeneralParticleSource;
class G4Event;

class PandaXGenerator : public BambooGenerator {
public:

  PandaXGenerator(const G4String &name);

  ~PandaXGenerator();

  virtual void GeneratePrimaries(G4Event * anEvent);

private:

  G4GeneralParticleSource * _particleGun;
  PandaXParticleSource * _pandaxParticleSource;
};
#endif //PANDAXGENERATOR_H
