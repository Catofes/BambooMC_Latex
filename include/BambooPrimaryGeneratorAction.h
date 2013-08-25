#ifndef BAMBOOPRIMARYGENERATORACTION_H
#define BAMBOOPRIMARYGENERATORACTION_H

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>

class BambooPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  BambooPrimaryGeneratorAction ();
  ~BambooPrimaryGeneratorAction ();

  // static access method
  static const BambooPrimaryGeneratorAction* Instance();

  virtual void GeneratePrimaries(G4Event*);

  const G4ParticleGun * GetParticleGun () const { return fParticleGun; }

private:
  static BambooPrimaryGeneratorAction * fgInstance;

  G4ParticleGun* fParticleGun;
  
};


#endif //BAMBOOPRIMARYGENERATORACTION_H
