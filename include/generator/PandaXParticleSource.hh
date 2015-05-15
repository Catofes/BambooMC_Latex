#ifndef PANDAXPARTICLESOURCE_H
#define PANDAXPARTICLESOURCE_H

#include <G4VPrimaryGenerator.hh>
#include <G4SPSPosDistribution.hh>
#include <G4SPSRandomGenerator.hh>

#include <TLorentzVector.h>

class G4Event;
class TRandom3;

class PandaXParticleSource : public G4VPrimaryGenerator
{
public:
  PandaXParticleSource();

  ~PandaXParticleSource();

  void GeneratePrimaryVertex(G4Event *pEvent);

private:

  void GenerateDoubleElectronFourMomentum(TLorentzVector & v1, TLorentzVector & v2);

  // nothing?
  G4SPSPosDistribution * _posGenerator;
  G4SPSRandomGenerator * _biasRndm;

  TRandom3 *_r3;
  
};
#endif // PANDAXPARTICLESOURCE_H
