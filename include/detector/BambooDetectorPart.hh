#ifndef BAMBOODETECTORPART_H
#define BAMBOODETECTORPART_H

#include <globals.hh>

class G4LogicalVolume;

class BambooDetectorPart {
public:
  BambooDetectorPart(const G4String &partName);

  virtual ~BambooDetectorPart() {}

  virtual G4bool construct(const G4String &, G4LogicalVolume*) = 0;

  G4String getName() const { return _partName; }

  void setParentLogicalVolume (BambooDetectorPart * parent);

  G4LogicalVolume * getLogicalVolume ();

private:
  G4String _partName;

  G4LogicalVolume * _partVolume;

  BambooDetectorPart * _parentPart;

  
};
#endif
