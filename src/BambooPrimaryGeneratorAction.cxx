#include "BambooPrimaryGeneratorAction.h"

#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>

BambooPrimaryGeneratorAction * BambooPrimaryGeneratorAction::fgInstance = 0;

const BambooPrimaryGeneratorAction * BambooPrimaryGeneratorAction::Instance() {
  return fgInstance;
}

BambooPrimaryGeneratorAction::BambooPrimaryGeneratorAction ()
  : G4VUserPrimaryGeneratorAction(),
    fParticleGun(0) {
  fParticleGun = new G4ParticleGun(1);

  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();

  G4String particleName;
  G4ParticleDefinition * particle = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(6.*MeV);

  fgInstance = this;
}

BambooPrimaryGeneratorAction::~BambooPrimaryGeneratorAction() {
  delete fParticleGun;
  fgInstance = 0;
}

void BambooPrimaryGeneratorAction::GeneratePrimaries(G4Event * anEvent) {
  fParticleGun->SetParticlePosition(G4ThreeVector());
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
