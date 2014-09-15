#include <G4RunManager.hh>
#include <G4String.hh>

#include <QBBC.hh>

#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif

#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#endif

#include "BambooControl.hh"
#include "BambooDetectorConstruction.hh"
#include "BambooGlobalVariables.hh"
#include "physics/BambooPhysicsFactory.hh"
#include "generator/BambooGeneratorFactory.hh"

void usage(const char *);

int main(int argc, char * argv[])
{
  BambooControl::getControl()->setup(argc, argv);

  G4cout << "Control loaded." << G4endl;
  G4RunManager* runManager = new G4RunManager;

  runManager->SetUserInitialization(new BambooDetectorConstruction);

  G4VModularPhysicsList* physicsList = BambooPhysicsFactory::Instance()
    ->createPhysics(BambooGlobalVariables::Instance()->getPhysicsName());

  runManager->SetUserInitialization(physicsList);

  G4VUserPrimaryGeneratorAction * generator = BambooGeneratorFactory::Instance()
    ->createGenerator(BambooGlobalVariables::Instance()->getGeneratorName());

  runManager->SetUserAction(generator);

  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

#ifdef G4UI_USE
  if (!BambooControl::MacroFileName.isNull()) {
    G4String command = "/control/execute " + BambooControl::MacroFileName;
    G4UImanager * uiManager = G4UImanager::GetUIpointer();
    uiManager->ApplyCommand(command);
  }

  if (BambooControl::INTERACTIVE) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    ui->SessionStart();
    delete ui;
  }
#endif

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;
  return 0;
}

void usage(const char * exeName) {
  std::cout << "Usage: " << exeName << " [macro]" << std::endl;
}
