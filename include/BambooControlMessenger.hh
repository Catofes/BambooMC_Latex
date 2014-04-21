#ifndef BAMBOOCONTROLMESSENGER_H
#define BAMBOOCONTROLMESSENGER_H

#include <G4UImessenger.hh>

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class BambooControl;

class BambooControlMessenger: public G4UImessenger
{
public:
  BambooControlMessenger(BambooControl * control);
  ~BambooControlMessenger();
  
  void SetNewValue(G4UIcommand * command, G4String newValue);

private:
  BambooControl * _control;
};
#endif
