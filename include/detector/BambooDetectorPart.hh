#ifndef BAMBOODETECTORPART_H
#define BAMBOODETECTORPART_H

#include <globals.hh>

class G4LogicalVolume;
class G4VPhysicalVolume;

class BambooDetectorPart {
public:
  BambooDetectorPart(const G4String &partName);

  virtual ~BambooDetectorPart() {}

  virtual G4bool construct() = 0;

  G4String getName() const { return _partName; }

  void setParentLogicalVolume (BambooDetectorPart * parent);

  G4LogicalVolume * getLogicalVolume ();

  G4VPhysicalVolume * getPhysicalVolume();

protected:
  G4String _partName;

  G4LogicalVolume * _partLogicalVolume;

  G4VPhysicalVolume * _partPhysicalVolume;

  BambooDetectorPart * _parentPart;

  
};
#endif
