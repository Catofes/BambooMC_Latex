#ifndef BAMBOOANALYSIS_H
#define BAMBOOANALYSIS_H

#include <globals.hh>

class G4UserRunAction;
class G4UserEventAction;
class G4UserSteppingAction;

class BambooAnalysis {
public:
  BambooAnalysis(const G4String &name);

  virtual ~BambooAnalysis() {}

  G4String getName() const { return _name; }

  G4UserRunAction * getRunAction();

  G4UserEventAction * getEventAction();

  G4UserSteppingAction * getSteppingAction();

protected:
  G4String _name;

  G4UserRunAction * _runAction;

  G4UserEventAction * _eventAction;

  G4UserSteppingAction * _steppingAction;
};
#endif
