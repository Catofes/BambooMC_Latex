#ifndef PANDAXPARTICLESOURCE_H
#define PANDAXPARTICLESOURCE_H

#include <G4VPrimaryGenerator.hh>
#include <G4SPSPosDistribution.hh>
#include <G4SPSRandomGenerator.hh>

class G4Event;

class PandaXParticleSource : public G4VPrimaryGenerator
{
public:
  PandaXParticleSource();

  ~PandaXParticleSource();

  void GeneratePrimaryVertex(G4Event *pEvent);

private:

  // nothing?
  G4SPSPosDistribution * _posGenerator;
  G4SPSRandomGenerator * _biasRndm;
  
};
#endif // PANDAXPARTICLESOURCE_H
