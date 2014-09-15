#ifndef SIMPLEGPSGENERATOR_H
#define SIMPLEGPSGENERATOR_H

#include "generator/BambooGenerator.hh"

class G4GeneralParticleSource;
class G4Event;

class SimpleGPSGenerator : public BambooGenerator {
public:

  SimpleGPSGenerator(const G4String &name);

  ~SimpleGPSGenerator();

  virtual void GeneratePrimaries(G4Event * anEvent);

private:

  G4GeneralParticleSource * _particleGun;

};
#endif //SIMPLEGPSGENERATOR_H
