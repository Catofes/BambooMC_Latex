#ifndef BAMBOOPRIMARYGENERATORACTION_H
#define BAMBOOPRIMARYGENERATORACTION_H

#include <G4VUserPrimaryGeneratorAction.hh>

class G4ParticleGun;
class G4Event;

class BambooPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:

  BambooPrimaryGeneratorAction();
  virtual ~BambooPrimaryGeneratorAction();  
  virtual void GeneratePrimaries(G4Event* anEvent);

private:
  G4ParticleGun* _particleGun;
};

#endif
