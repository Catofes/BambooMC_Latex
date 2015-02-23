#ifndef PANDAXRUNACTION_H
#define PANDAXRUNACTION_H

#include <G4UserRunAction.hh>

class G4Run;
class PandaXDataManager;

class PandaXRunAction : public G4UserRunAction {
public:
  PandaXRunAction (PandaXDataManager * manager);
  
  virtual ~PandaXRunAction ();

  virtual void BeginOfRunAction (const G4Run *aRun);

  virtual void EndOfRunAction (const G4Run *aRun);

private:

  PandaXDataManager * _dataManager;
};

#endif // PANDAXRUNACTION_H
