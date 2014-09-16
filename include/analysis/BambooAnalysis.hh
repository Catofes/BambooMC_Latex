#ifndef BAMBOOANALYSIS_H
#define BAMBOOANALYSIS_H

#include <globals.hh>

class BambooAnalysis {
public:
  BambooAnalysis(const G4String &name);

  virtual ~BambooAnalysis() {}

  G4String getName() const { return _name; }

protected:
  G4String _name;

};
#endif
