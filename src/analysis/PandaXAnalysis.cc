#include "analysis/PandaXAnalysis.hh"
#include "analysis/BambooAnalysisFactory.hh"
#include "analysis/PandaXRunAction.hh"
#include "analysis/PandaXDataManager.hh"

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
  PandaXDataManager * dm = new PandaXDataManager();
  _runAction = new PandaXRunAction(dm);
}

PandaXAnalysis::~PandaXAnalysis()
{
}
