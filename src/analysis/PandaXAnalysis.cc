#include "analysis/PandaXAnalysis.hh"
#include "analysis/BambooAnalysisFactory.hh"
#include "analysis/PandaXRunAction.hh"
#include "analysis/PandaXEventAction.hh"
#include "analysis/PandaXDataManager.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"

// anonymous namespace to register the PandaXAnalysis
namespace {
  BambooAnalysis * createPandaXAnalysis ()
  {
    return new PandaXAnalysis("PandaXAnalysis");
  }

  const std::string PandaXAnalysisName("PandaXAnalysis");

  const bool registered = BambooAnalysisFactory::Instance()->registerAnalysis(PandaXAnalysisName, createPandaXAnalysis);
}


PandaXAnalysis::PandaXAnalysis (const G4String &name)
  : BambooAnalysis(name)
{
  int dEnergyDeposition = BambooGlobalVariables::Instance()->getAnalysisParameterAsInt("EnableEnergyDeposition");
  int dFlatSurfaceFlux = BambooGlobalVariables::Instance()->getAnalysisParameterAsInt("EnableFlatSurfaceFlux");
  bool enableEnergyDeposition(true);
  bool enableFlatSurfaceFlux(false);
  if (dEnergyDeposition == 0) {
    enableEnergyDeposition = false;
  }
  if (dFlatSurfaceFlux == 1) {
    enableFlatSurfaceFlux = true;
  }
  if (dEnergyDeposition!=0&&dEnergyDeposition!=1) {
    G4cout << "EnableEnergyDeposition can only be 0(false) or 1(true). 1 will be used in the simulation." << G4endl;
  }
  if (dFlatSurfaceFlux!=0&&dFlatSurfaceFlux!=1) {
    G4cout << "EnableFlatSurfaceFlux can only be 0(false) or 1(true). 0 will be used in the simulation." << G4endl;
  }
  PandaXDataManager * dm = new PandaXDataManager(enableEnergyDeposition, enableFlatSurfaceFlux);
  _runAction = new PandaXRunAction(dm);
  _eventAction = new PandaXEventAction(dm);
}

PandaXAnalysis::~PandaXAnalysis()
{
}
