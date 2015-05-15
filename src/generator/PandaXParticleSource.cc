#include "generator/PandaXParticleSource.hh"

#include <G4Event.hh>
#include <G4PrimaryVertex.hh>
#include <G4PrimaryParticle.hh>
#include <G4ParticleDefinition.hh>
#include <G4Electron.hh>

#include <TRandom3.h>
#include <TMath.h>

PandaXParticleSource::PandaXParticleSource()
{
  _biasRndm = new G4SPSRandomGenerator();
  _posGenerator = new G4SPSPosDistribution();
  _posGenerator->SetBiasRndm(_biasRndm);
  // the quick dirty hack of the position.
  // should be controlled with the messenger
  _posGenerator->SetPosDisType("Volume");
  _posGenerator->SetPosDisShape("Cylinder");
  _posGenerator->SetCentreCoords(G4ThreeVector(51*m, 0, 1*m));
  _posGenerator->SetRadius(1.6*m);
  _posGenerator->SetHalfZ(1.2*m);
  _posGenerator->ConfineSourceToVolume("HpXenonGasDetector");
  _r3 = new TRandom3();
  _r3->SetSeed(0);
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
  G4cout << "(" << pos.x()/m << ", " << pos.y()/m << ", " << pos.z()/m << ")" << G4endl; 
  // quick hack of two electons
  TLorentzVector v1, v2;
  GenerateDoubleElectronFourMomentum(v1, v2);
  G4ParticleDefinition * electronDefinition = G4Electron::ElectronDefinition();
  G4PrimaryParticle * e1 = new G4PrimaryParticle(electronDefinition);
  e1->Set4Momentum(v1.Px(), v1.Py(), v1.Pz(), v1.E());
  vertex->SetPrimary(e1);
  G4PrimaryParticle * e2 = new G4PrimaryParticle(electronDefinition);
  e2->Set4Momentum(v2.Px(), v2.Py(), v2.Pz(), v2.E());
  G4cout<< v1.E() << " + " << v2.E() << " = " << v1.E() + v2.E() << G4endl;
  vertex->SetPrimary(e2);
  G4cout << "Total number of particle: " << vertex->GetNumberOfParticle() << G4endl;
  pEvent->AddPrimaryVertex(vertex);
}

void PandaXParticleSource::GenerateDoubleElectronFourMomentum(TLorentzVector & v2, TLorentzVector & v3)
{
  // 0 -> Xe136
  // 1 -> Ba136
  // 2 -> e-
  // 3 -> e-
  // 4 -> combination of 2 and 3
  double m0 = 135.907219 * 931.494061; // MeV
  double m1 = 135.9045759 * 931.494061; // MeV
  double m2 = 0.510998928; // MeV

  double m4; // 2m2 < m4 < m0 - m1
  double e1, e2;
  double p1, p4;
  double e2p, p2p;			// p - prime
  double theta_1, phi_1;
  double theta_2, phi_2;
  double ctheta_1, ctheta_2;
  double p1x, p1y, p1z;
  m4 = _r3->Uniform(2*m2, m0-m1);
  e1 = 0.5*(m0*m0-m4*m4+m1*m1)/m0;
  e2 = m0 - e1;
  p4 = TMath::Sqrt(e1*e1-m1*m1);
  ctheta_1 = _r3->Uniform(-1, 1);
  theta_1 = TMath::ACos(ctheta_1);
  phi_1 = _r3->Uniform(0, 2.*TMath::Pi());
  p1 = p4;
  p1x = p1*TMath::Sin(theta_1)*TMath::Cos(phi_1);
  p1y = p1*TMath::Sin(theta_1)*TMath::Sin(phi_1);
  p1z = p1*ctheta_1;
  TLorentzVector v1;
  v1.SetPxPyPzE(-p1x, -p1y, -p1z, e2);
  e2p = m4/2;
  p2p = TMath::Sqrt(e2p*e2p - m2*m2);
  //    v1.Boost(-(v1.BoostVector()));
  ctheta_2 = _r3->Uniform(-1, 1);
  theta_2 = TMath::ACos(ctheta_2);
  phi_2 = _r3->Uniform(0, 2.*TMath::Pi());
  v2.SetPxPyPzE(p2p*TMath::Sin(theta_2)*TMath::Cos(phi_2), p2p*TMath::Sin(theta_2)*TMath::Sin(phi_2), p2p*ctheta_2, e2p);
  v3.SetPxPyPzE(-p2p*TMath::Sin(theta_2)*TMath::Cos(phi_2), -p2p*TMath::Sin(theta_2)*TMath::Sin(phi_2), -p2p*ctheta_2, e2p);
  //    cout << v2.M() << " " << v3.M() << endl;
  v2.Boost(v1.BoostVector());
  v3.Boost(v1.BoostVector());
}
