#include "analysis/PandaXDataManager.hh"
#include "analysis/PandaXEnergyDepositionHit.hh"
#include "analysis/PandaXFlatSurfaceFluxHit.hh"

#include <TFile.h>
#include <TTree.h>

#include <G4Event.hh>
#include <G4ParticleDefinition.hh>
#include <G4SDManager.hh>
#include <G4THitsMap.hh>

#include <iostream>

PandaXDataManager::PandaXDataManager (bool enableEnergyDeposition, bool enableFlatSurfaceFlux)
  : _rootFile(0), _mcTree(0), _recordEnergyDeposition(enableEnergyDeposition), _recordFlatSurfaceFlux(enableFlatSurfaceFlux)
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
  if (_recordEnergyDeposition) {
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
  if (_recordFlatSurfaceFlux) {
    _mcTree->Branch("nTracks", &_nTracks);
    _mcTree->Branch("trackEnergy", &_trackEnergy);
    _mcTree->Branch("px", &_px);
    _mcTree->Branch("py", &_py);
    _mcTree->Branch("pz", &_pz);
    _mcTree->Branch("surface", &_surface);
  }
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
  if (_recordEnergyDeposition) {
    for (int i=0; i<nHitCollections; ++i) {
      G4VHitsCollection * hitsCollection = hCthis->GetHC(i);
      if (hitsCollection->GetName().contains("EnergyDepositionHits")) {
        PandaXEnergyDepositionHitsCollection * hC = (PandaXEnergyDepositionHitsCollection*)hitsCollection;
        for (size_t j=0; j<hitsCollection->GetSize(); ++j) {
          PandaXEnergyDepositionHit * hit = (PandaXEnergyDepositionHit *)hC->GetHit(j);
          _trackId.push_back(hit->getTrackId());
          _parentId.push_back(hit->getParentId());
          _type.push_back(hit->getType());
          _parentType.push_back(hit->getParent());
          _creatorProcess.push_back(hit->getCreatorProcess());
          _depositionProcess.push_back(hit->getDepositionProcess());
          _volume.push_back(hitsCollection->GetSDname());
          _xd.push_back(hit->getX()/mm);
          _yd.push_back(hit->getY()/mm);
          _zd.push_back(hit->getZ()/mm);
          _td.push_back(hit->getT()/s);
          _energy.push_back(hit->getEnergy()/keV);
          _totalEnergy += (hit->getEnergy())/keV;
          _nHits++;
        }
      }
    }
  }
  if (_recordFlatSurfaceFlux) {
    for (int i=0; i<nHitCollections; ++i) {
      G4VHitsCollection * hitsCollection = hCthis->GetHC(i);
      if (hitsCollection->GetName().contains("FlatSurfaceFluxHits")) {
        PandaXFlatSurfaceFluxHitsCollection * hC = (PandaXFlatSurfaceFluxHitsCollection*)hitsCollection;
        for (size_t j=0; j<hitsCollection->GetSize(); ++j) {
          PandaXFlatSurfaceFluxHit * hit = (PandaXFlatSurfaceFluxHit *)hC->GetHit(j);
          G4ThreeVector momentum = hit->getMomentum();
          _trackEnergy.push_back(hit->getEnergy()/keV);
          _px.push_back(momentum.x()/keV);
          _py.push_back(momentum.y()/keV);
          _pz.push_back(momentum.z()/keV);
          _surface.push_back(hit->getSurface());
          _nTracks++;
        }
      }
    }
  }
  if ((_recordEnergyDeposition&&_nHits>0)
      ||(_recordFlatSurfaceFlux&&_nTracks>0))
    _mcTree->Fill();
}

void PandaXDataManager::resetData()
{
  if (_recordEnergyDeposition) {
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
  if (_recordFlatSurfaceFlux) {
    _nTracks = 0;
    _trackEnergy.clear();
    _px.clear();
    _py.clear();
    _pz.clear();
    _surface.clear();
  }
}
