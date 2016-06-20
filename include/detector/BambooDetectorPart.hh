#ifndef BAMBOODETECTORPART_H
#define BAMBOODETECTORPART_H

#include <globals.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>

class G4LogicalVolume;

class G4VPhysicalVolume;

class BambooDetectorPart
{
public:
    BambooDetectorPart(const G4String &typeName);

    virtual ~BambooDetectorPart()
    { }

    virtual G4bool construct() = 0;

    G4String getName() const
    { return _partName; }

    G4String getType() const
    { return _typeName; }

    void setParent(BambooDetectorPart *parent);

    void setName(const G4String &name);

    G4LogicalVolume *getLogicalVolume();

    G4LogicalVolume *getContainerLogicalVolume();

    G4VPhysicalVolume *getPhysicalVolume();

protected:
    G4String _partName;

    G4String _typeName;

    G4LogicalVolume *_partLogicalVolume;

    G4LogicalVolume *_partContainerLogicalVolume;

    G4VPhysicalVolume *_partPhysicalVolume;

    BambooDetectorPart *_parentPart;


};

#endif
