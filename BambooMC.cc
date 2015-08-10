#include <G4RunManager.hh>
#include <G4String.hh>

#include <QBBC.hh>
#include <G4PhysListFactory.hh>

#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif

#ifdef G4UI_USE
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#endif

#include <sstream>
#include <string>

#include "BambooControl.hh"
#include "BambooDetectorConstruction.hh"
#include "BambooGlobalVariables.hh"
#include "physics/BambooPhysicsFactory.hh"
#include "analysis/BambooAnalysisFactory.hh"
#include "generator/BambooGeneratorFactory.hh"

void usage(const char *);

int main(int argc, char * argv[])
{
  BambooControl::getControl()->setup(argc, argv);

  G4cout << "Control loaded." << G4endl;
  G4RunManager* runManager = new G4RunManager;

  runManager->SetRunIDCounter(BambooGlobalVariables::Instance()->getRunNumber());
  runManager->SetUserInitialization(new BambooDetectorConstruction);

  if (BambooGlobalVariables::Instance()->getPhysicsName().find("Physics")==std::string::npos) {
    G4PhysListFactory * physListFactory = new G4PhysListFactory();
    G4VUserPhysicsList * physicsList = physListFactory->GetReferencePhysList(BambooGlobalVariables::Instance()->getPhysicsName());
    runManager->SetUserInitialization(physicsList);
  } else {
    G4VModularPhysicsList* physicsList = BambooPhysicsFactory::Instance()
      ->createPhysics(BambooGlobalVariables::Instance()->getPhysicsName());

    runManager->SetUserInitialization(physicsList);
  }

  G4VUserPrimaryGeneratorAction * generator = BambooGeneratorFactory::Instance()
    ->createGenerator(BambooGlobalVariables::Instance()->getGeneratorName());

  runManager->SetUserAction(generator);

  // analysis
  BambooAnalysis * analysis = BambooAnalysisFactory::Instance()
    ->createAnalysis(BambooGlobalVariables::Instance()->getAnalysisName());

  G4UserRunAction * runAction = analysis->getRunAction();
  G4UserEventAction * eventAction = analysis->getEventAction();
  G4UserSteppingAction * steppingAction = analysis->getSteppingAction();
  G4UserStackingAction * stackingAction = analysis->getStackingAction();
  G4UserTrackingAction * trackingAction = analysis->getTrackingAction();

  if (runAction!=0) {
    runManager->SetUserAction(runAction);
  }
  if (eventAction!=0) {
    runManager->SetUserAction(eventAction);
  }
  if (steppingAction!=0) {
    runManager->SetUserAction(steppingAction);
  }
  if (stackingAction!=0) {
    runManager->SetUserAction(stackingAction);
  }
  if (trackingAction!=0) {
    runManager->SetUserAction(trackingAction);
  }

  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

#ifdef G4UI_USE
  if (!BambooControl::MacroFileName.isNull()) {
    G4String command = "/control/execute " + BambooControl::MacroFileName;
    G4UImanager * uiManager = G4UImanager::GetUIpointer();
    G4cout << command << G4endl;
    uiManager->ApplyCommand(command);
  }

  if (BambooControl::NUM_EVENTS) {
    std::ostringstream ss;
    ss << BambooControl::NUM_EVENTS;
    G4String command = "/run/beamOn " + ss.str();
    G4UImanager * uiManager = G4UImanager::GetUIpointer();
    G4cout << command << G4endl;
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
