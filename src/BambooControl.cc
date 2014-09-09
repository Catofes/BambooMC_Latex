#include "BambooControl.hh"
#include "BambooControlMessenger.hh"
#include "BambooGlobalVariables.hh"

#include <unistd.h>
#include <string>
#include <sstream>
#include <G4UImanager.hh>

G4int BambooControl::DETECTOR_TYPE = 0;
G4bool BambooControl::INTERACTIVE = false;
G4int BambooControl::NUM_EVENTS = 0;
G4String BambooControl::MacroFileName = "";
G4String BambooControl::DataFileName = "";
G4String BambooControl::XmlFileName = "";

BambooControl * BambooControl::theControl = 0;

BambooControl* BambooControl::getControl()
{
  if (theControl==0) {
    theControl = new BambooControl();
  }
  return theControl;
}

BambooControl::BambooControl()
{
  new BambooControlMessenger(this);
}

BambooControl::~BambooControl()
{
}

void BambooControl::setup(int argc, char * argv[])
{
  extern char * optarg;
  std::stringstream ss;
  while (true) {
    const int option = getopt(argc, argv, "f:m:n:o:x:i");
    if(option == -1) break;
    switch (option) {
    case 'f':
    case 'm':
      BambooControl::MacroFileName = optarg;
      break;
    case 'n':
      ss.clear();
      ss.str(optarg);
      ss >> BambooControl::NUM_EVENTS;
      break;
    case 'o':
      BambooControl::DataFileName = optarg;
      break;
    case 'x':
      BambooControl::XmlFileName = optarg;
      BambooGlobalVariables::Instance()->loadXMLFile(BambooControl::XmlFileName);
      break;
    case 'i':
      INTERACTIVE = true;
      break;
    }
  }

  if (optind < argc - 1) {
    G4cout << "No steering file will be used." << G4endl;
  } else if (optind == argc-1) {
    G4String steeringFileName = argv[optind];
    G4UImanager::GetUIpointer()->ApplyCommand(G4String("/control/execute ") + steeringFileName);
  }
}
