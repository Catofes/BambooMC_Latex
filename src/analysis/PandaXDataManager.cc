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
}

void PandaXDataManager::save()
{
  _mcTree->Write();
}

void PandaXDataManager::fill()
{
  _mcTree->Fill();
}
