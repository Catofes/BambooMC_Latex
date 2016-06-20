#include "analysis/PandaXDataManager.hh"
#include "analysis/PandaXEnergyDepositionHit.hh"
#include "analysis/PandaXFlatSurfaceFluxHit.hh"

#include <TFile.h>
#include <TTree.h>

#include <G4Event.hh>
#include <G4ParticleDefinition.hh>
#include <G4SDManager.hh>
#include <G4THitsMap.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>

#include <iostream>

PandaXDataManager *PandaXDataManager::_instance = 0;

TemporaryParticle::TemporaryParticle()
        : _type("unknown"), _id(0), _energy(0), _px(0), _py(0), _pz(0),
          _x(0), _y(0), _z(0)
{
}

TemporaryParticle::~TemporaryParticle()
{
}

const std::string &TemporaryParticle::getParticleType() const
{
    return _type;
}

void TemporaryParticle::setParticleType(const std::string type)
{
    _type = type;
}

int TemporaryParticle::getId() const
{
    return _id;
}

void TemporaryParticle::setId(int i)
{
    _id = i;
}

double TemporaryParticle::getEnergy() const
{
    return _energy;
}

void TemporaryParticle::setEnergy(double e)
{
    _energy = e;
}

double TemporaryParticle::getPx() const
{
    return _px;
}

void TemporaryParticle::setPx(double v)
{
    _px = v;
}

double TemporaryParticle::getPy() const
{
    return _py;
}

void TemporaryParticle::setPy(double v)
{
    _py = v;
}

double TemporaryParticle::getPz() const
{
    return _pz;
}

void TemporaryParticle::setPz(double v)
{
    _pz = v;
}

double TemporaryParticle::getX() const
{
    return _x;
}

void TemporaryParticle::setX(double v)
{
    _x = v;
}

double TemporaryParticle::getY() const
{
    return _y;
}

void TemporaryParticle::setY(double v)
{
    _y = v;
}

double TemporaryParticle::getZ() const
{
    return _z;
}

void TemporaryParticle::setZ(double v)
{
    _z = v;
}

PandaXDataManager::PandaXDataManager(bool enableEnergyDeposition, bool enableFlatSurfaceFlux,
                                     bool enablePrimaryParticle)
        : _rootFile(0), _mcTree(0),
          _recordEnergyDeposition(enableEnergyDeposition),
          _recordFlatSurfaceFlux(enableFlatSurfaceFlux),
          _recordPrimaryParticle(enablePrimaryParticle),
          _saveNullEvent(false),
          _partialSaved(false)
{
    _instance = this;
}

PandaXDataManager::~PandaXDataManager()
{
    if (_rootFile != 0) {
        _rootFile->Close();
        delete _rootFile;
    }
}

void PandaXDataManager::book(const std::string &name)
{
    _rootFile = new TFile(name.c_str(), "RECREATE");
    if (_rootFile->IsZombie()) {
        std::cerr << "PandaXDataManager::book: ";
        std::cerr << "problem creating ROOT file." << std::endl << std::endl;
        delete _rootFile;
        _rootFile = 0;
        return;
    }
    _mcTree = new TTree("mcTree", "Tree with MC info");
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
        _mcTree->Branch("trackName", &_trackName);
        _mcTree->Branch("trackParent", &_trackParent);
        _mcTree->Branch("px", &_px);
        _mcTree->Branch("py", &_py);
        _mcTree->Branch("pz", &_pz);
        _mcTree->Branch("surface", &_surface);
    }
    if (_recordPrimaryParticle) {
        _mcTree->Branch("nPrimaries", &_nPrimaries);
        _mcTree->Branch("primaryType", &_primaryType);
        _mcTree->Branch("primaryId", &_primaryId);
        _mcTree->Branch("primaryEnergy", &_primaryEnergy);
        _mcTree->Branch("primaryPx", &_primaryPx);
        _mcTree->Branch("primaryPy", &_primaryPy);
        _mcTree->Branch("primaryPz", &_primaryPz);
        _mcTree->Branch("primaryX", &_primaryX);
        _mcTree->Branch("primaryY", &_primaryY);
        _mcTree->Branch("primaryZ", &_primaryZ);
    }
}

void PandaXDataManager::save()
{
    _mcTree->Write();
}

void PandaXDataManager::saveNullEvent(bool t)
{
    _saveNullEvent = t;
}

void PandaXDataManager::fillEvent(const G4Event *aEvent, bool partial)
{
    if (!partial && _partialSaved)
        return;
    G4HCofThisEvent *hCthis = aEvent->GetHCofThisEvent();
    int nHitCollections = hCthis->GetNumberOfCollections();
    resetData();
    if (_recordEnergyDeposition) {
        for (int i = 0; i < nHitCollections; ++i) {
            G4VHitsCollection *hitsCollection = hCthis->GetHC(i);
            if (hitsCollection->GetName().contains("EnergyDepositionHits")) {
                PandaXEnergyDepositionHitsCollection *hC = (PandaXEnergyDepositionHitsCollection *) hitsCollection;
                for (size_t j = 0; j < hitsCollection->GetSize(); ++j) {
                    PandaXEnergyDepositionHit *hit = (PandaXEnergyDepositionHit *) hC->GetHit(j);
                    _trackId.push_back(hit->getTrackId());
                    _parentId.push_back(hit->getParentId());
                    _type.push_back(hit->getType());
                    _parentType.push_back(hit->getParent());
                    _creatorProcess.push_back(hit->getCreatorProcess());
                    _depositionProcess.push_back(hit->getDepositionProcess());
                    _volume.push_back(hitsCollection->GetSDname());
                    _xd.push_back(hit->getX() / mm);
                    _yd.push_back(hit->getY() / mm);
                    _zd.push_back(hit->getZ() / mm);
                    _td.push_back(hit->getT() / s);
                    _energy.push_back(hit->getEnergy() / keV);
                    _totalEnergy += (hit->getEnergy()) / keV;
                    _nHits++;
                }
            }
        }
    }
    if (_recordFlatSurfaceFlux) {
        for (int i = 0; i < nHitCollections; ++i) {
            G4VHitsCollection *hitsCollection = hCthis->GetHC(i);
            if (hitsCollection->GetName().contains("FlatSurfaceFluxHits")) {
                PandaXFlatSurfaceFluxHitsCollection *hC = (PandaXFlatSurfaceFluxHitsCollection *) hitsCollection;
                for (size_t j = 0; j < hitsCollection->GetSize(); ++j) {
                    PandaXFlatSurfaceFluxHit *hit = (PandaXFlatSurfaceFluxHit *) hC->GetHit(j);
                    G4ThreeVector momentum = hit->getMomentum();
                    _trackEnergy.push_back(hit->getEnergy() / keV);
                    _trackName.push_back(hit->getTrackName());
                    _trackParent.push_back(hit->getParentName());
                    _px.push_back(momentum.x() / keV);
                    _py.push_back(momentum.y() / keV);
                    _pz.push_back(momentum.z() / keV);
                    _surface.push_back(hit->getSurface());
                    _nTracks++;
                }
            }
        }
    }
    if (_recordPrimaryParticle) {
        if (!partial || _particles.size() == 0) {
            // loop over all primary particles...
            int nVertex = aEvent->GetNumberOfPrimaryVertex();
            for (int iV = 0; iV < nVertex; ++iV) {
                G4PrimaryVertex *vertex = aEvent->GetPrimaryVertex(iV);
                int nParticles = vertex->GetNumberOfParticle();
                double x = vertex->GetX0();
                double y = vertex->GetY0();
                double z = vertex->GetZ0();
                for (int ip = 0; ip < nParticles; ++ip) {
                    G4PrimaryParticle *particle = vertex->GetPrimary(ip);
                    _primaryType.push_back(particle->GetParticleDefinition()->GetParticleName());
                    _primaryId.push_back(particle->GetTrackID());
                    _primaryEnergy.push_back(particle->GetTotalEnergy());
                    _primaryPx.push_back(particle->GetPx());
                    _primaryPy.push_back(particle->GetPy());
                    _primaryPz.push_back(particle->GetPz());
                    _primaryX.push_back(x);
                    _primaryY.push_back(y);
                    _primaryZ.push_back(z);
                    _nPrimaries++;
                }
            }
        } else {
            for (size_t ip = 0; ip < _particles.size(); ++ip) {
                TemporaryParticle &particle = _particles[ip];
                _primaryType.push_back(particle.getParticleType());
                _primaryId.push_back(particle.getId());
                _primaryEnergy.push_back(particle.getEnergy());
                _primaryPx.push_back(particle.getPx());
                _primaryPy.push_back(particle.getPy());
                _primaryPz.push_back(particle.getPz());
                _primaryX.push_back(particle.getX());
                _primaryY.push_back(particle.getY());
                _primaryZ.push_back(particle.getZ());
                _nPrimaries++;
            }
        }
        if (partial) {
            // clear the temporary particles
            _particles = _tparticles;
            _tparticles.clear();
        }
    }
    if ((_recordEnergyDeposition && _nHits > 0)
        || (_recordFlatSurfaceFlux && _nTracks > 0)
        || (_saveNullEvent && _recordPrimaryParticle))
        _mcTree->Fill();
    if (partial) {
        resetPartialEvent(aEvent);
    }
    _partialSaved = partial;
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
        _trackName.clear();
        _trackParent.clear();
        _px.clear();
        _py.clear();
        _pz.clear();
        _surface.clear();
    }
    if (_recordPrimaryParticle) {
        _nPrimaries = 0;
        _primaryType.clear();
        _primaryId.clear();
        _primaryEnergy.clear();
        _primaryPx.clear();
        _primaryPy.clear();
        _primaryPz.clear();
        _primaryX.clear();
        _primaryY.clear();
        _primaryZ.clear();
    }
    _trackMap.clear();
}

void PandaXDataManager::resetPartialEvent(const G4Event *aEvent)
{
    G4HCofThisEvent *hCthis = aEvent->GetHCofThisEvent();
    int nHitCollections = hCthis->GetNumberOfCollections();
    for (int i = 0; i < nHitCollections; ++i) {
        G4VHitsCollection *hitsCollection = hCthis->GetHC(i);
        if (hitsCollection->GetName().contains("EnergyDepositionHits")) {
            PandaXEnergyDepositionHitsCollection *hC = (PandaXEnergyDepositionHitsCollection *) hitsCollection;
            for (int i = 0; i < hC->entries(); ++i) {
                PandaXEnergyDepositionHit *hit = (*hC)[i];
                delete hit;
            }
            hC->GetVector()->clear();
        } else if (hitsCollection->GetName().contains("FlatSurfaceFluxHits")) {
            PandaXFlatSurfaceFluxHitsCollection *hC = (PandaXFlatSurfaceFluxHitsCollection *) hitsCollection;
            for (int i = 0; i < hC->entries(); ++i) {
                PandaXFlatSurfaceFluxHit *hit = (*hC)[i];
                delete hit;
            }
            hC->GetVector()->clear();
        }
    }
}

std::map<int, std::string> &PandaXDataManager::getTrackMap()
{
    return _trackMap;
}

PandaXDataManager *PandaXDataManager::Instance()
{
    if (_instance == 0) {
        return new PandaXDataManager();
    }
    return _instance;
}

void PandaXDataManager::addTemporaryParticle(const TemporaryParticle &tp)
{
    _tparticles.push_back(tp);
}
