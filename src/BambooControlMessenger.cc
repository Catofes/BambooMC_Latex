#include "BambooControlMessenger.hh"
#include "BambooControl.hh"

#include <G4UIdirectory.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4UIcmdWithAString.hh>
#include <G4Tokenizer.hh>

BambooControlMessenger::BambooControlMessenger (BambooControl * control)
{
  _control = control;
  _pandaXDirectory = new G4UIdirectory("/PandaX/");
  _pandaXDirectory->SetGuidance("PandaX specific commands");

  _detectorTypeCmd = new G4UIcmdWithAString("/PandaX/Type" ,this);
  _detectorTypeCmd->SetGuidance("Specify detector type. (default is MiniPandaX");
  _detectorTypeCmd->SetParameterName("DetectorType", true);
  _detectorTypeCmd->SetCandidates("MiniPandaX PandaX PandaX3inchPMTs");
  _detectorTypeCmd->SetDefaultValue("MiniPandaX");
  _detectorTypeCmd->AvailableForStates(G4State_PreInit);

  _bottomTeflonConeReflectorCmd = new G4UIcmdWithABool("/PandaX/BuildBottomTeflonConeReflector", this);
  _bottomTeflonConeReflectorCmd->SetGuidance("If yes, add Polyhedra-Polycone shape Teflon reflector");
  _bottomTeflonConeReflectorCmd->SetParameterName("BottomTeflonConeReflector", true); 
  _bottomTeflonConeReflectorCmd->AvailableForStates(G4State_PreInit);
}

BambooControlMessenger::~BambooControlMessenger()
{
  delete _detectorTypeCmd;
  delete _bottomTeflonConeReflectorCmd;
  delete _pandaXDirectory;
}

void BambooControlMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
  if (command == _detectorTypeCmd) {
    if (newValue == "MiniPandaX") {
      BambooControl::DETECTOR_TYPE = 0;
    } else if (newValue == "PandaX") {
      BambooControl::DETECTOR_TYPE = 1;
    } else if (newValue == "PandaX3inchPMTs") {
      BambooControl::DETECTOR_TYPE = 2;
    }
  } else if (command == _bottomTeflonConeReflectorCmd) {
    BambooControl::BOTTOM_TEFLON_CONE_REFLECTOR = _bottomTeflonConeReflectorCmd->GetNewBoolValue(newValue);
  }
}

