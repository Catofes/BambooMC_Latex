//
// Created by herbertqiao on 9/18/15.
//
#ifndef BAMBOOMC_EXTPRIMARYGENERATOR_H
#define BAMBOOMC_EXTPRIMARYGENERATOR_H

#include "generator/BambooGenerator.hh"
#include "G4Event.hh"
#include <vector>
#include "G4ParticleGun.hh"



class ParticleInfo {
public:
    ParticleInfo() { }

    ~ParticleInfo() { }

    int ParticleType = 0;
    double px = 0;
    double py = 0;
    double pz = 0;
    double Time = 0;
};

class ExtPrimaryGenerator : public BambooGenerator {

public:
    ExtPrimaryGenerator(const G4String &name);

    ~ExtPrimaryGenerator();

    void GeneratePrimaries(G4Event *event);

    void GenerateLoc(G4ThreeVector *loc);

private:
    void LoadFile();

    G4String _InputFile = "";
    std::vector<std::vector<ParticleInfo>> _GunData;
    G4ParticleGun *_gun;
    unsigned int _num = 0;
};

#endif