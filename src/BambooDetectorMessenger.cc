#include "BambooDetectorMessenger.hh"
#include "BambooDetectorConstruction.hh"

#include <G4UIcommand.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

BambooDetectorMessenger::BambooDetectorMessenger (BambooDetectorConstruction * detector)
{
  _bambooDetector = detector;
}

BambooDetectorMessenger::~BambooDetectorMessenger ()
{
  
}

void BambooDetectorMessenger::SetNewValue (G4UIcommand * command, G4String value)
{
}
