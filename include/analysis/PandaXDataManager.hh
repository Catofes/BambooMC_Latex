#ifndef PANDAXDATAMANAGER_H
#define PANDAXDATAMANAGER_H

#include "analysis/PandaXRunAction.hh"

#include <string>
#include <vector>

class TFile;
class TTree;

class G4Event;

class PandaXDataManager {

  friend class PandaXRunAction;
  friend class PandaXEventAction;

public:
  PandaXDataManager(bool enableEnergyDeposition = true, bool enableFlatSurfaceFlux = false, bool enablePrimaryParticle = false);

  ~PandaXDataManager();

  void book(const std::string & name = "pandaxout.root");

  void save();

  void saveNullEvent(bool t);

  void fillEvent(const G4Event * aEvent);

private:
  void resetData();

  TFile * _rootFile;
  TTree * _mcTree;

  int _runId;
  int _eventId;

  // variables for the energy deposition.
  int _nHits;
  double _totalEnergy;
  std::vector<int> _trackId;
  std::vector<int> _parentId;
  std::vector<std::string > _type;
  std::vector<std::string > _parentType;
  std::vector<std::string > _creatorProcess;
  std::vector<std::string > _depositionProcess;
  std::vector<std::string > _volume;
  std::vector<double> _xd;
  std::vector<double> _yd;
  std::vector<double> _zd;
  std::vector<double> _td;
  std::vector<double> _energy;

  // variables for flux counts
  int _nTracks;
  std::vector<double> _trackEnergy;
  std::vector<std::string> _trackName;
  std::vector<double> _px;
  std::vector<double> _py;
  std::vector<double> _pz;
  std::vector<int> _surface;

  // variables for primary particle
  int _nPrimaries;
  std::vector<std::string > _primaryType;
  std::vector<int> _primaryId;
  std::vector<double> _primaryEnergy;
  std::vector<double> _primaryPx;
  std::vector<double> _primaryPy;
  std::vector<double> _primaryPz;
  std::vector<double> _primaryX;
  std::vector<double> _primaryY;
  std::vector<double> _primaryZ;

  bool _recordEnergyDeposition;
  bool _recordFlatSurfaceFlux;
  bool _recordPrimaryParticle;
  bool _saveNullEvent;
};
#endif
