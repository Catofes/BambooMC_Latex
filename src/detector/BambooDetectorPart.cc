#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include "detector/BambooDetectorPart.hh"

BambooDetectorPart::BambooDetectorPart (const G4String &partName)
  : _partName(partName)
{
}

void BambooDetectorPart::setParentLogicalVolume (BambooDetectorPart * parent)
{
  _parentPart = parent;
}

G4LogicalVolume * BambooDetectorPart::getLogicalVolume ()
{
  return _partLogicalVolume;
}

G4VPhysicalVolume * BambooDetectorPart::getPhysicalVolume ()
{
  return _partPhysicalVolume;
}
