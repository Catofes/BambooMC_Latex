#ifndef PANDAXDATAMANAGER_H
#define PANDAXDATAMANAGER_H

#include "analysis/PandaXRunAction.hh"

#include <string>
#include <vector>

class TFile;
class TTree;

class PandaXDataManager {

  friend class PandaXRunAction;
  friend class PandaXEventAction;

public:
  PandaXDataManager();

  ~PandaXDataManager();

  void book(const std::string & name = "pandaxout.root");

  void save();

  void fill();

private:
  TFile * _rootFile;
  TTree * _mcTree;

  int _eventId;
  int _nHits;
  std::vector<int> _trackId;
  std::vector<int> _parentId;
  std::vector<int> _ancestorId;
  std::vector<std::string > _type;
  std::vector<std::string > _parentType;
  std::vector<std::string > _ancestorType;
  std::vector<std::string > _createProcess;
  std::vector<std::string > _depositionProcess;
  std::vector<std::string > _volume;
  std::vector<double> _xd;
  std::vector<double> _yd;
  std::vector<double> _zd;
  std::vector<double> _td;
  std::vector<double> _energy;
};
#endif
