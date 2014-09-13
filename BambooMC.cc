#include <G4RunManager.hh>
#include <G4String.hh>

#include <QBBC.hh>

#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif

#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#endif

#include "BambooControl.hh"
#include "BambooDetectorConstruction.hh"
#include "BambooPhysicsList.hh"
#include "BambooPrimaryGeneratorAction.hh"

void usage(const char *);

int main(int argc, char * argv[])
{
  BambooControl::getControl()->setup(argc, argv);

  G4RunManager* runManager = new G4RunManager;

  runManager->SetUserInitialization(new BambooDetectorConstruction);

  //  runManager->SetUserInitialization(new BambooPhysicsList);
  G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserAction(new BambooPrimaryGeneratorAction);

  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

#ifdef G4UI_USE
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
