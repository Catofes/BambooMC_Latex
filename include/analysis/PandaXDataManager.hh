#ifndef PANDAXDATAMANAGER_H
#define PANDAXDATAMANAGER_H

#include "analysis/PandaXRunAction.hh"

#include <string>

class TFile;
class TTree;

class PandaXDataManager {

  friend class PandaXRunAction;

public:
  PandaXDataManager();

  ~PandaXDataManager();

  void book(const std::string & name = "pandaxout.root");

  void save();

  void fill();

private:
  TFile * _rootFile;
  TTree * _mcTree;
  
};
#endif
