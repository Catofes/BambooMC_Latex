#include "analysis/PandaXLXeSensitiveDetector.hh"

#include <G4Step.hh>
#include <G4HCofThisEvent.hh>
#include <G4TouchableHistory.hh>
#include <G4VProcess.hh>

PandaXLXeSensitiveDetector::PandaXLXeSensitiveDetector(const G4String & name)
  : G4VSensitiveDetector(name), _hitsCollection(0)
{
  collectionName.push_back("LXeHits");
}

PandaXLXeSensitiveDetector::~PandaXLXeSensitiveDetector()
{
}

void PandaXLXeSensitiveDetector::Initialize(G4HCofThisEvent* hc)
{
  _hitsCollection = new PandaXLXeHitsCollection(SensitiveDetectorName, collectionName[0]);
  hc->AddHitsCollection(GetCollectionID(0), _hitsCollection);
  _particleTypes.clear();
}

void PandaXLXeSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
}

G4bool PandaXLXeSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.)
    return false;

  PandaXLXeHit * hit = new PandaXLXeHit();
  int hitId = aStep->GetTrack()->GetTrackID();
  hit->setTrackId(hitId);
  if (_particleTypes.find(hitId)==_particleTypes.end()) {
    _particleTypes[hitId] = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  }
  int parentId = aStep->GetTrack()->GetParentID();
  hit->setParentId(parentId);
  if (parentId && _particleTypes.find(parentId)!=_particleTypes.end()) {
    hit->setParent(_particleTypes[parentId]);
  }
  hit->setEnergy(edep/keV);
  hit->setX(aStep->GetPreStepPoint()->GetPosition().x()/mm);
  hit->setY(aStep->GetPreStepPoint()->GetPosition().y()/mm);
  hit->setZ(aStep->GetPreStepPoint()->GetPosition().z()/mm);
  hit->setT(aStep->GetPreStepPoint()->GetGlobalTime()/s);
  hit->setType(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  if (aStep->GetTrack()->GetCreatorProcess()) {
    hit->setCreatorProcess(aStep->GetTrack()->GetCreatorProcess()->GetProcessName());
  }
  hit->setDepositionProcess(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
  _hitsCollection->insert(hit);
  return true;
}
