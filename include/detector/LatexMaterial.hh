//
// Created by herbertqiao on 6/10/16.
//

#ifndef BAMBOOMC_LATEXMATERIAL_H
#define BAMBOOMC_LATEXMATERIAL_H

#include "detector/BambooMaterial.hh"

class LatexMaterial : public BambooMaterial
{
public:
    LatexMaterial(const G4String &name);

    virtual void defineMaterials();


};


#endif //BAMBOOMC_LATEXMATERIAL_H
