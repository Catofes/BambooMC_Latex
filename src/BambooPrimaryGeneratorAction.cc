#include "BambooPrimaryGeneratorAction.hh"

#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
#include <globals.hh>


BambooPrimaryGeneratorAction::BambooPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction () {
  G4int n_particle = 1;
  _particleGun = new G4ParticleGun(n_particle);
 
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
 
  G4String particleName;
  G4ParticleDefinition * particle = particleTable->FindParticle(particleName="gamma");
  G4cout << particle << G4endl;
  _particleGun->SetParticleDefinition(particle);
  G4cout << "set particle definition" << G4endl;
  //Default energy,position,momentum
  _particleGun->SetParticleEnergy(511.*keV);
  _particleGun->SetParticlePosition(G4ThreeVector(0.0 , 0.0, -20.0*cm));
  _particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

BambooPrimaryGeneratorAction::~BambooPrimaryGeneratorAction(){
    delete _particleGun;
}

void BambooPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  _particleGun->GeneratePrimaryVertex(anEvent);
}
