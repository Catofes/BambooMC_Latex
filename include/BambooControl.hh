#ifndef BAMBOOCONTROL_H
#define BAMBOOCONTROL_H

#include <globals.hh>

class BambooControlMessenger;

class BambooControl
{
  friend class BambooControlMessenger;
public:
  static BambooControl * getControl();

  virtual ~BambooControl();

  void setup(int argc, char * argv[]);

  static G4int DETECTOR_TYPE;
  static G4bool BOTTOM_TEFLON_CONE_REFLECTOR;
  static G4bool INTERACTIVE;
  static G4int NUM_EVENTS;
  static G4String MacroFileName;
  static G4String DataFileName;

private:

  BambooControl();

  static BambooControl * theControl;
};
#endif
