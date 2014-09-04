#include <G4LogicalVolume.hh>
#include "detector/BambooDetectorPart.hh"

BambooDetectorPart::BambooDetectorPart (const G4String &partName)
  : _partName(partName)
{
}

void BambooDetectorPart::setParentLogicalVolume (G4LogicalVolume * parent)
{
  _parent = parent;
}
