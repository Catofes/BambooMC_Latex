#ifndef PANDAXSTEPPINGACTION_HH
#define PANDAXSTEPPINGACTION_HH

#include "analysis/PandaXDataManager.hh"

#include <G4UserSteppingAction.hh>

class G4Step;

class PandaXSteppingAction : public G4UserSteppingAction
{
public:
  PandaXSteppingAction(PandaXDataManager * dm);

  ~PandaXSteppingAction();

  virtual void UserSteppingAction(const G4Step * aStep);

  void setDataManager (PandaXDataManager * manager);

private:
  PandaXDataManager * _manager;
};

#endif //PANDAXSTEPPINGACTION_HH
