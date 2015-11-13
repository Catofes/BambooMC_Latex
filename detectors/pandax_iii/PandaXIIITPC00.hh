#ifndef PANDAXIIITPC00_H
#define PANDAXIIITPC00_H

#include "detector/BambooDetectorPart.hh"

class G4Material;

class PandaXIIITPC00 : public BambooDetectorPart
{

public:

  PandaXIIITPC00(const G4String &name);

  virtual G4bool construct();

private:

  // parameters for shaping rings
  double _ringInnerRadius;
  double _ringThickness;
  double _ringHeight;
  double _ringSpacing;

  int _nRings;                  // number of rings for one field cage

  // parameters for the cathode frame and cathode
  double _cathodeFrameHeight;
  double _cathodeHeight;

  // parameters for the supporter
  double _supporterWidth;
  double _supporterThickness;
  int _nSupportingBars;             // number of supporters in one cage

  // about the sensitive gas
  bool _buildSenstivieGas;

  /// direction of the electric field
  /// 0: no field; 1: x; 2: y; 3: z
  int _electricFieldDirection;

  double _electricField;
 
  
  G4LogicalVolume * _ringsLog;
  G4VPhysicalVolume * _ringsPhys;

  G4LogicalVolume * _cathodeLog;
  G4VPhysicalVolume * _cathodePhys;

  double _readoutPlateThickness;
  G4LogicalVolume * _readoutPlateLog;

  G4LogicalVolume * _upXeLogicalVolume;
  G4LogicalVolume * _downXeLogicalVolume;
};


#endif // PANDAXIIITPC00_H

