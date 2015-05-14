#include "generator/PandaXParticleSource.hh"

#include <G4Event.hh>
#include <G4PrimaryVertex.hh>

PandaXParticleSource::PandaXParticleSource()
{
  _biasRndm = new G4SPSRandomGenerator();
  _posGenerator = new G4SPSPosDistribution();
  _posGenerator->SetBiasRndm(_biasRndm);
}

PandaXParticleSource::~PandaXParticleSource()
{
  delete _biasRndm;
  delete _posGenerator;
}

void PandaXParticleSource::GeneratePrimaryVertex(G4Event * pEvent)
{
  G4ThreeVector pos = _posGenerator->GenerateOne();
  G4PrimaryVertex * vertex = new G4PrimaryVertex(pos, 0.0);

  // quick hack of two electons
  
  pEvent->AddPrimaryVertex(vertex);
}
