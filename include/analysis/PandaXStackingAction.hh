#ifndef PANDAXSTACKINGACTION_HH
#define PANDAXSTACKINGACTION_HH

#include "analysis/PandaXDataManager.hh"

#include <G4UserStackingAction.hh>

class G4Track;

class PandaXStackingAction : public G4UserStackingAction
{
public:
  PandaXStackingAction(PandaXDataManager * dm);

  ~PandaXStackingAction();

  virtual G4ClassificationOfNewTrack ClassifyNewTrack (const G4Track *aTrack);

  virtual void NewStage();

  virtual void PrepareNewEvent();

  void setDataManager (PandaXDataManager * manager);

private:
  PandaXDataManager * _manager;
};

#endif //PANDAXSTACKINGACTION_HH
