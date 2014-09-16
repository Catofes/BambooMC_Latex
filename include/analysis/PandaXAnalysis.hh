#ifndef PANDAXANALYSIS_H
#define PANDAXANALYSIS_H

#include "analysis/BambooAnalysis.hh"

class PandaXAnalysis : public BambooAnalysis {
public:
  PandaXAnalysis(const G4String &name);

  ~PandaXAnalysis();
  
};

#endif // PANDAXANALYSIS_H
