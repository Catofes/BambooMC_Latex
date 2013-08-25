#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

#include <G4VisExecutive.hh>
#include <QGSP_BERT_HP.hh>

#include "BambooDetectorConstruction.h"
#include "BambooPrimaryGeneratorAction.h"

int main(int argc, char * argv[]) {
  // construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // // set mandatory initialization classes
  runManager->SetUserInitialization(new BambooDetectorConstruction());

  G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
  runManager->SetUserInitialization(physicsList);

  // // set mandatory user action class
  runManager->SetUserAction(new BambooPrimaryGeneratorAction());

  // // initialize G4 kernel
  runManager->Initialize();

  G4VisManager * visManager = new G4VisExecutive;
  visManager->Initialize();
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  // UI->ApplyCommand("/run/verbose 1");
  // UI->ApplyCommand("/event/verbose 1");
  // UI->ApplyCommand("/tracking/verbose 1");

  // // start a run
  // int numberOfEvent = 3;
  // runManager->BeamOn(numberOfEvent);
  G4UIExecutive * ui = new G4UIExecutive(argc, argv);
  ui->SessionStart();
  delete ui;
  // job termination
  delete runManager;
  return 0;
}
