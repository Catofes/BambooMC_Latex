#ifndef PANDAXLXESENSITIVEDETECTOR_H
#define PANDAXLXESENSITIVEDETECTOR_H

#include <G4VSensitiveDetector.hh>
#include "analysis/PandaXLXeHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PandaXLXeSensitiveDetector : public G4VSensitiveDetector
{
public:
  PandaXLXeSensitiveDetector (const G4String &name);
  ~PandaXLXeSensitiveDetector ();

  virtual void Initialize(G4HCofThisEvent *);

  virtual void EndOfEvent (G4HCofThisEvent *);

protected:
  virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *roHist);

private:
  PandaXLXeHitsCollection * _hitsCollection;
};

#endif // PANDAXLXESENSITIVEDETECTOR_H
