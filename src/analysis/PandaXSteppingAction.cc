#include "analysis/PandaXSteppingAction.hh"

#include <G4SystemOfUnits.hh>
#include <G4Step.hh>

PandaXSteppingAction::PandaXSteppingAction(PandaXDataManager *dm)
  : _manager(dm)
{
}

PandaXSteppingAction::~PandaXSteppingAction()
{
}


void PandaXSteppingAction::UserSteppingAction (const G4Step * aStep)
{
  G4Track * aTrack = aStep->GetTrack();
  const G4ParticleDefinition *def = aTrack->GetParticleDefinition();

  // the nucleus without kinematic energy should be marked as postpone
  if (def->GetParticleType () == "nucleus" && aTrack->GetTrackStatus() == fStopButAlive) {
    aTrack->SetTrackStatus(fPostponeToNextEvent);
    aTrack->SetGlobalTime(0.0*second);
  } else if (def->GetParticleType () == "nucleus" && aTrack->GetTrackStatus() == fPostponeToNextEvent) {
    aTrack->SetTrackStatus(fStopButAlive);
  }
}
