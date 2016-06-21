//
// Created by herbertqiao on 6/10/16.
//

#ifndef BAMBOOMC_LATEXWORLD_H
#define BAMBOOMC_LATEXWORLD_H


#include "detector/BambooDetectorPart.hh"

class LatexWorld : public BambooDetectorPart
{
public:
    LatexWorld(const G4String &name);

    virtual G4bool construct();

private:
    double _halfX;
    double _halfY;
    double _halfZ;
    bool _magnetic_field;
    double _magnetic_field_B;

};

#endif //BAMBOOMC_LATEXWORLD_H