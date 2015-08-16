#include "analysis/PandaXStackingAction.hh"

#include <G4Track.hh>
#include <G4StackManager.hh>
#include <G4EventManager.hh>

PandaXStackingAction::PandaXStackingAction(PandaXDataManager *dm)
  : _manager(dm)
{
}

PandaXStackingAction::~PandaXStackingAction()
{
}

G4ClassificationOfNewTrack PandaXStackingAction::ClassifyNewTrack (const G4Track *aTrack)
{
  const G4ParticleDefinition *def = aTrack->GetParticleDefinition();
  if (aTrack->GetTrackStatus() == fPostponeToNextEvent) {
    TemporaryParticle tp;
    tp.setParticleType(def->GetParticleName());
    tp.setEnergy(aTrack->GetTotalEnergy());
    tp.setPx(aTrack->GetMomentum().x());
    tp.setPy(aTrack->GetMomentum().y());
    tp.setPz(aTrack->GetMomentum().z());
    tp.setX(aTrack->GetPosition().x());
    tp.setY(aTrack->GetPosition().y());
    tp.setZ(aTrack->GetPosition().z());
    _manager->addTemporaryParticle(tp);
    return fWaiting;
  }
  return fUrgent;
}

void PandaXStackingAction::NewStage ()
{
  // all the urgent tracks have been processed. So we can save the results.
  _manager -> fillEvent(G4EventManager::GetEventManager()->GetConstCurrentEvent(), true);
}

void PandaXStackingAction::PrepareNewEvent()
{
}
