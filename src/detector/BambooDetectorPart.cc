#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include "detector/BambooDetectorPart.hh"

BambooDetectorPart::BambooDetectorPart(const G4String &partName)
        : _partName(partName), _typeName(""), _partLogicalVolume(0), _partContainerLogicalVolume(0),
          _partPhysicalVolume(0), _parentPart(0)
{
}

void BambooDetectorPart::setParent(BambooDetectorPart *parent)
{
    _parentPart = parent;
}

void BambooDetectorPart::setName(const G4String &name)
{
    _partName = name;
}

G4LogicalVolume *BambooDetectorPart::getLogicalVolume()
{
    return _partLogicalVolume;
}

G4VPhysicalVolume *BambooDetectorPart::getPhysicalVolume()
{
    return _partPhysicalVolume;
}

G4LogicalVolume *BambooDetectorPart::getContainerLogicalVolume()
{
    return _partContainerLogicalVolume;
}
