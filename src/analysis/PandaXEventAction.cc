#include "analysis/PandaXEventAction.hh"
#include "analysis/PandaXDataManager.hh"

#include "BambooGlobalVariables.hh"

#include <G4Event.hh>

PandaXEventAction::PandaXEventAction (PandaXDataManager * manager)
  : _dataManager(manager)
{
  _dataManager->_eventId = 0;
}

PandaXEventAction::~PandaXEventAction ()
{
}

void PandaXEventAction::BeginOfEventAction (const G4Event *aEvent)
{
  _dataManager->_eventId++;
}

void PandaXEventAction::EndOfEventAction (const G4Event *aEvent)
{
  _dataManager->fill();
}
