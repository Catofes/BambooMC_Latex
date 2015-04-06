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
  int enableFluxScorer = BambooGlobalVariables::Instance()->getAnalysisParameterAsInt("EnableFluxScorer");
  PandaXDataManager * dm = new PandaXDataManager();
  if (enableFluxScorer==1) {
    dm->enableFluxScorer(true);
  } else if (enableFluxScorer != 0) {
    G4cout << "EnableFluxScorer can only be 0 or 1. 0 will be used in the simulation." << G4endl;
  }
  _runAction = new PandaXRunAction(dm);
  _eventAction = new PandaXEventAction(dm);
}

PandaXAnalysis::~PandaXAnalysis()
{
}
