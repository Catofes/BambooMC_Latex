#include "analysis/PandaXDataManager.hh"
#include "analysis/PandaXLXeHit.hh"

#include <TFile.h>
#include <TTree.h>

#include <G4Event.hh>
#include <G4ParticleDefinition.hh>

#include <iostream>

PandaXDataManager::PandaXDataManager ()
  : _rootFile(0), _mcTree(0)
{
}

PandaXDataManager::~PandaXDataManager ()
{
  if(_rootFile!=0) {
    _rootFile->Close();
    delete _rootFile;
  }
}

void PandaXDataManager::book(const std::string & name)
{
  _rootFile = new TFile (name.c_str(), "RECREATE");
  if (_rootFile->IsZombie()) {
    std::cerr << "PandaXDataManager::book: ";
    std::cerr << "problem creating ROOT file." << std::endl << std::endl;
    delete _rootFile;
    _rootFile = 0;
    return;
  }
  _mcTree = new TTree ("mcTree", "Tree with MC info");
  _mcTree->Branch("runId", &_runId, "runId/I");
  _mcTree->Branch("eventId", &_eventId, "eventId/I");
  _mcTree->Branch("nHits", &_nHits, "nHits/I");
  _mcTree->Branch("trackId", &_trackId);
  _mcTree->Branch("parentId", &_parentId);
  _mcTree->Branch("type", &_type);
  _mcTree->Branch("parentType", &_parentType);
  _mcTree->Branch("creatorProcess", &_creatorProcess);
  _mcTree->Branch("depositionProcess", &_depositionProcess);
  _mcTree->Branch("volume", &_volume);
  _mcTree->Branch("totalEnergy", &_totalEnergy);
  _mcTree->Branch("xd", &_xd);
  _mcTree->Branch("yd", &_yd);
  _mcTree->Branch("zd", &_zd);
  _mcTree->Branch("td", &_td);
  _mcTree->Branch("energy", &_energy);
}

void PandaXDataManager::save()
{
  _mcTree->Write();
}

void PandaXDataManager::fillEvent(const G4Event *aEvent)
{
  G4HCofThisEvent * hCthis = aEvent->GetHCofThisEvent();
  int nHitCollections = hCthis->GetNumberOfCollections();
  resetData();
  for (int i=0; i<nHitCollections; ++i) {
    PandaXLXeHitsCollection * hitsCollection = (PandaXLXeHitsCollection *)hCthis->GetHC(i);
    for (size_t j=0; j<hitsCollection->GetSize(); ++j) {
      PandaXLXeHit * hit = (PandaXLXeHit *)hitsCollection->GetHit(j);
      _trackId.push_back(hit->getTrackId());
      _parentId.push_back(hit->getParentId());
      _type.push_back(hit->getType());
      _parentType.push_back(hit->getParent());
      _creatorProcess.push_back(hit->getCreatorProcess());
      _depositionProcess.push_back(hit->getDepositionProcess());
      _volume.push_back(hitsCollection->GetSDname());
      _xd.push_back(hit->getX());
      _yd.push_back(hit->getY());
      _zd.push_back(hit->getZ());
      _td.push_back(hit->getT());
      _energy.push_back(hit->getEnergy());
      _totalEnergy += hit->getEnergy();
      _nHits++;
    }
  }
  if (_nHits>0)
    _mcTree->Fill();
}

void PandaXDataManager::resetData()
{
  _nHits = 0;
  _trackId.clear();
  _parentId.clear();
  _type.clear();
  _parentType.clear();
  _creatorProcess.clear();
  _depositionProcess.clear();
  _volume.clear();
  _totalEnergy = 0;
  _xd.clear();
  _yd.clear();
  _zd.clear();
  _td.clear();
  _energy.clear();
}
