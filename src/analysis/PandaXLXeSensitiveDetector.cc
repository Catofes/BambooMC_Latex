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
  hit->setTrackId(aStep->GetTrack()->GetTrackID());
  hit->setParentId(aStep->GetTrack()->GetParentID());
  hit->setEnergy(edep);
  hit->setX(aStep->GetPreStepPoint()->GetPosition().x());
  hit->setY(aStep->GetPreStepPoint()->GetPosition().y());
  hit->setZ(aStep->GetPreStepPoint()->GetPosition().z());
  hit->setT(aStep->GetPreStepPoint()->GetGlobalTime());
  hit->setType(aStep->GetTrack()->GetParticleDefinition()->GetParticleName().c_str());
  hit->setProcess(aStep->GetTrack()->GetCreatorProcess()->GetProcessName().c_str());
  _hitsCollection->insert(hit);
  return true;
}
