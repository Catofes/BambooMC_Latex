//
// Created by herbertqiao on 1/13/16.
//

#ifndef BAMBOOMC_PANDAXIIIVESSELSUPPORTER_H
#define BAMBOOMC_PANDAXIIIVESSELSUPPORTER_H

#include "detector/BambooDetectorPart.hh"

class G4Material;


class PandaXIIIVesselSupporter : public BambooDetectorPart
{
public:

    PandaXIIIVesselSupporter(const G4String &name);

    virtual G4bool construct();

private:
    double _steelShellInnerRadius;
    double _steelShellInnerHeight;
    double _steelShellBarrelThickness;
    double _steelShellEndThickness;
    double _shiftX;
    double _shiftY;
    double _shiftZ;

    G4LogicalVolume * _steelShellLogicalVolume;
    G4VPhysicalVolume * _steelShellPhysicalVolume;
    G4LogicalVolume * _waterTubLogicalVolume;
    G4VPhysicalVolume * _waterTubPhysicalVolume;

};


#endif //BAMBOOMC_PANDAXIIIVESSELSUPPORTER_H
