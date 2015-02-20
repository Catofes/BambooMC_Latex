#ifndef PANDAXEVENTACTION_H
#define PANDAXEVENTACTION_H

#include <G4UserEventAction.hh>

class G4Event;
class PandaXDataManager;

class PandaXEventAction : public G4UserEventAction {
public:
  PandaXEventAction (PandaXDataManager * manager);
  
  virtual ~PandaXEventAction ();

  virtual void BeginOfEventAction (const G4Event *aEvent);

  virtual void EndOfEventAction (const G4Event *aEvent);

private:

  PandaXDataManager * _dataManager;
};

#endif // PANDAXEVENTACTION_H
