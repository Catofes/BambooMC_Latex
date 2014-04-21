#ifndef BAMBOODETECTORMESSENGER_H
#define BAMBOODETECTORMESSENGER_H

#include <G4UImessenger.hh>
#include <globals.hh>

class BambooDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcommand;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;

class BambooDetectorMessenger : public G4UImessenger
{
public:

  BambooDetectorMessenger(BambooDetectorConstruction*);

  virtual ~BambooDetectorMessenger();

  virtual void SetNewValue(G4UIcommand *, G4String);

private:

  BambooDetectorConstruction * _bambooDetector;
  G4UIdirectory * _detectorDir;
  G4UIdirectory * _volumesDir;
  
};

#endif
