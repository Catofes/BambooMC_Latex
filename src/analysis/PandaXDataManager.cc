#include "analysis/PandaXDataManager.hh"

#include <TFile.h>
#include <TTree.h>

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
  _mcTree->Branch("eventId", &_eventId, "eventId/I");
  _mcTree->Branch("nHits", &_nHits, "nHits/I");
  _mcTree->Branch("trackId", &_trackId);
  _mcTree->Branch("parentId", &_parentId);
  _mcTree->Branch("ancestorId", &_ancestorId);
  _mcTree->Branch("type", &_type);
  _mcTree->Branch("parentType", &_parentType);
  _mcTree->Branch("ancestorType", &_ancestorType);
  _mcTree->Branch("createProcess", &_createProcess);
  _mcTree->Branch("depositionProcess", &_depositionProcess);
  _mcTree->Branch("volume", &_volume);
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

void PandaXDataManager::fill()
{
  _mcTree->Fill();
}
