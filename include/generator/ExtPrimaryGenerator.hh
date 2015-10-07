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
    ParticleInfo() {
        ParticleType = 0;
        px = 0;
        py = 0;
        pz = 0;
        Time = 0;
    }

    ~ParticleInfo() { }

    int ParticleType;
    double px;
    double py;
    double pz;
    double Time;
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
    unsigned int _num;
    G4String _loc_type;
    double _loc_x;
    double _loc_y;
    double _loc_z;
    bool _rotate;

};

#endif
