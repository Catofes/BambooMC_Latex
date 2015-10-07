//
// Created by herbertqiao on 9/18/15.
//

#include "generator/ExtPrimaryGenerator.hh"
#include "generator/BambooGeneratorFactory.hh"
#include "BambooUtils.hh"
#include "BambooGlobalVariables.hh"
#include "G4RandomTools.hh"
#include "G4ParticleMomentum.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include <fstream>
#include <stdexcept>
#include <cmath>
#include "G4strstreambuf.hh"

namespace {
    BambooGenerator *createHQPrimaryGenerator() {
        return new ExtPrimaryGenerator("ExtPrimaryGenerator");
    }

    const std::string ExtPrimaryGenerator("ExtPrimaryGenerator");
    const bool registered = BambooGeneratorFactory::Instance()->registerGenerator(ExtPrimaryGenerator,
                                                                                  createHQPrimaryGenerator);
}


ExtPrimaryGenerator::ExtPrimaryGenerator(const G4String &name) : BambooGenerator(name) {
    _num = 0;
    this->_InputFile = G4String(BambooGlobalVariables::Instance()->getGeneratorParameterAsString("InputFile"));
    if (this->_InputFile.empty()) {
        throw std::runtime_error("Wrong Input File. Can't Generator Any Particle.");
    }
    this->LoadFile();
    this->_loc_type = G4String(BambooGlobalVariables::Instance()->getGeneratorParameterAsString("Type"));
    if (this->_loc_type == "Function");
    else if (this->_loc_type == "Loc");
    else if (this->_loc_type.empty())
        this->_loc_type = G4String("Loc");
    else {
        throw std::runtime_error("Wrong Type. Can't Know Particle Generate Location.");
    }
    this->_loc_x = BambooGlobalVariables::Instance()->getGeneratorParameterAsDouble("LocX");
    this->_loc_y = BambooGlobalVariables::Instance()->getGeneratorParameterAsDouble("LocY");
    this->_loc_z = BambooGlobalVariables::Instance()->getGeneratorParameterAsDouble("LocZ");
    if (BambooGlobalVariables::Instance()->getGeneratorParameterAsInt("RndRotate") == 1)
        this->_rotate = true;
    else
        this->_rotate = false;
    this->_gun = new G4ParticleGun();
}


void ExtPrimaryGenerator::LoadFile() {
    std::fstream file;
    try {
        file.open(this->_InputFile);
        int particle_num;
        while (true) {
            if (file >> particle_num) {
                std::vector<ParticleInfo> particles;
                for (int i = 0; i < particle_num; i++) {
                    ParticleInfo particle;
                    string null;
                    file >> null;
                    file >> particle.ParticleType;
                    file >> null;
                    file >> null;
                    file >> particle.px;
                    file >> particle.py;
                    file >> particle.pz;
                    file >> null;
                    file >> particle.Time;
                    file >> null;
                    file >> null;
                    file >> null;
                    file >> null;
                    file >> null;
                    if (particle.ParticleType == 9802004) {
                        particle.ParticleType = 1000020040;
                    }
                    particles.push_back(particle);
                }
                this->_GunData.push_back(particles);
            } else
                break;
        }
    } catch (...) {
        throw std::runtime_error("Parse Input File Error.");
    };
    std::cout << "Load Events:" << _GunData.size() << std::endl;
}

ExtPrimaryGenerator::~ExtPrimaryGenerator() {
}

void ExtPrimaryGenerator::GenerateLoc(G4ThreeVector *loc) {
    DetectorParameters dp = BambooGlobalVariables::Instance()
            ->findDetectorPartParameters("HpXenonGasDetector");
    double _vesselInnerRadius = BambooUtils::evaluate(dp.getParameterAsString("vessel_inner_radius"));
    double _vesselBarrelThickness = BambooUtils::evaluate(dp.getParameterAsString("vessel_barrel_thickness"));
    double _vesselEndThickness = BambooUtils::evaluate(dp.getParameterAsString("vessel_end_thickness"));

    double _vesselInnerHeight = BambooUtils::evaluate(dp.getParameterAsString("vessel_inner_height"));

    double _shiftX = BambooUtils::evaluate(dp.getParameterAsString("shift_x"));
    double _shiftY = BambooUtils::evaluate(dp.getParameterAsString("shift_y"));
    double _shiftZ = BambooUtils::evaluate(dp.getParameterAsString("shift_z"));

    double V_barrel = pi * (pow((_vesselInnerRadius + _vesselBarrelThickness), 2) - pow(_vesselInnerRadius, 2)) *
                      _vesselInnerHeight;
    double V_end = pi * pow(_vesselBarrelThickness + _vesselInnerRadius, 2) * _vesselEndThickness;
    double V_total = V_barrel + V_end * 2;
    if (G4UniformRand() < V_barrel / V_total) {
        loc->setX((1 - G4UniformRand() * 2) * _vesselInnerHeight / 2.0);
        double theta = G4UniformRand() * 2 * pi;
        double r = sqrt(
                G4UniformRand() * (pow(_vesselBarrelThickness + _vesselInnerRadius, 2) - pow(_vesselInnerRadius, 2)) +
                (pow(_vesselInnerRadius, 2)));
        loc->setY(r * sin(theta));
        loc->setZ(r * cos(theta));
    } else {
        double theta = G4UniformRand() * 2 * pi;
        double r = sqrt(G4UniformRand() * pow(_vesselBarrelThickness + _vesselInnerRadius, 2));
        if (G4UniformRand() < 0.5)
            loc->setX(G4UniformRand() * _vesselEndThickness + _vesselInnerHeight / 2.0);
        else
            loc->setX(-G4UniformRand() * _vesselEndThickness - _vesselInnerHeight / 2.0);
        loc->setY(r * sin(theta));
        loc->setZ(r * cos(theta));
    }
    loc->setX(loc->getX() + _shiftX);
    loc->setY(loc->getY() + _shiftY);
    loc->setZ(loc->getZ() + _shiftZ);
}

void ExtPrimaryGenerator::GeneratePrimaries(G4Event *event) {
    if (_num > _GunData.size()) {
        throw std::runtime_error("Can't Generate Event. Data Out of Range.");
    }
    G4ThreeVector loc(0, 0, 0);
    if (_loc_type == "Function")
        this->GenerateLoc(&loc);
    else
        loc.set(_loc_x, _loc_y, _loc_z);
    std::vector<ParticleInfo> particles = _GunData[_num];
    G4double costheta = -1 + 2 * G4UniformRand();
    //randomize phi
    G4double phi = 360 * G4UniformRand() * degree;
    G4double sintheta = sqrt(1. - costheta * costheta);

    for (unsigned int i = 0; i < particles.size(); i++) {
        ParticleInfo p = particles[i];
        G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition *particle = particleTable->FindParticle(p.ParticleType);

        this->_gun->SetParticleDefinition(particle);
        G4ParticleMomentum momentum(p.px, p.py, p.pz);
        if (_rotate) {
            double pMag = sqrt(p.px * p.px + p.py * p.py + p.pz * p.pz);
            momentum.set(pMag * sintheta * cos(phi), pMag * sintheta * sin(phi), pMag * costheta);
        }
        G4strstreambuf *oldBuffer = dynamic_cast<G4strstreambuf *>(G4cout.rdbuf(0));
        this->_gun->SetParticleMomentum(momentum * GeV);
        G4cout.rdbuf(oldBuffer);
        this->_gun->SetParticlePosition(loc * mm);
        this->_gun->SetParticleTime(p.Time * nanosecond);
        this->_gun->GeneratePrimaryVertex(event);
    }
    _num++;
}
