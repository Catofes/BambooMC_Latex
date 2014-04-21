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
}

BambooControlMessenger::~BambooControlMessenger()
{
}

void BambooControlMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
}

