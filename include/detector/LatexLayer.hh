#include "detector/BambooDetectorPart.hh"

class G4Material;

class LatexLayer : public BambooDetectorPart
{
public:

    LatexLayer(const G4String &name);

    virtual G4bool construct();

private:
    double _halfX;
    double _halfY;
    double _latexZ;
    double _PETZ;
    double _shiftX;
    double _shiftY;
    double _shiftZ;

};