#include "analysis/PandaXAnalysis.hh"
#include "analysis/BambooAnalysisFactory.hh"

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
}

PandaXAnalysis::~PandaXAnalysis()
{
}
