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
  int eventId = aEvent->GetEventID();
  _dataManager->_eventId = eventId;
  if (eventId%100000 == 0) {
    G4cout << "Processing event " << eventId << "..." << G4endl;
  }
}

void PandaXEventAction::EndOfEventAction (const G4Event *aEvent)
{
  _dataManager->fillEvent(aEvent);
}
