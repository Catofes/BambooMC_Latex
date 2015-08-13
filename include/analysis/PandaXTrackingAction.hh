#ifndef PANDAXTRACKINGACTION_HH
#define PANDAXTRACKINGACTION_HH

#include "analysis/PandaXDataManager.hh"

#include <G4UserTrackingAction.hh>

class G4Track;

class PandaXTrackingAction : public G4UserTrackingAction
{
public:
  PandaXTrackingAction(PandaXDataManager * dm);

  ~PandaXTrackingAction();

  virtual void PreUserTrackingAction (const G4Track * aTrack);

  void setDataManager (PandaXDataManager * manager);

private:
  PandaXDataManager * _manager;
};

#endif //PANDAXTRACKINGACTION_HH
