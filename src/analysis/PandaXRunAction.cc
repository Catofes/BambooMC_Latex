#include "analysis/PandaXRunAction.hh"
#include "analysis/PandaXDataManager.hh"
#include "BambooGlobalVariables.hh"

#include <G4Run.hh>

PandaXRunAction::PandaXRunAction (PandaXDataManager *manager)
  : _dataManager(manager)
{
}

PandaXRunAction::~PandaXRunAction ()
{
}

void PandaXRunAction::BeginOfRunAction (const G4Run * aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  const string & name = BambooGlobalVariables::Instance()->getOutDataName();
  _dataManager->book(name);
  _dataManager->_runId = aRun->GetRunID();
}

void PandaXRunAction::EndOfRunAction (const G4Run * aRun)
{
  G4cout << "\n----------End of Run " << aRun->GetRunID() << "----------\n";
  _dataManager->save();
}
