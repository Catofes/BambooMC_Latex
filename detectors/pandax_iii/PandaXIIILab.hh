#ifndef PANDAXIIILAB_H
#define PANDAXIIILAB_H

#include "detector/BambooDetectorPart.hh"

//! The class to simulate the laboratory for PandaX-III
/*!
 * This class is used to simulate the laboratory for PandaX.
 * The inner length, width and height need to be provided by the user.
 * The thickness of concrete/rock walls will also be provided by the user.
 * The rock walls are outside of the concrete walls.
 * The default unit of length is mm.
 */
class PandaXIIILab : public BambooDetectorPart
{

public:

  PandaXIIILab(const G4String &name);

  virtual G4bool construct();

private:
  
  double _length;               //!< length of the inner space of the lab
  double _width;                //!< width of the inner space of the lab
  double _height;               //!< height of the inner space of the lab

  double _rockWallThickness;    //!< thickness of the rock wall
  double _rockRoofThickness;    //!< thickness of the rock roof
  double _rockFloorThickness;   //!< thickness of the rock floor

  double _concreteWallThickness;   //!< thickness of the concrete wall 
  double _concreteRoofThickness;   //!< thickness of the concrete roof 
  double _concreteFloorThickness;  //!< thickness of the concrete floor

  double _platformHeight;       //!< height of the platform
  double _platformLength;       //!< length of the platform

  double _poolLength;           //!< length of the water pool
  double _poolWidth;            //!< width of the water pool
  double _poolDepth;            //!< depth of the water pool

  G4LogicalVolume * _rockWallLogicalVolume; //!< the logical volume, rock wall
  G4LogicalVolume * _concreteWallLogicalVolume; //!< the logical volume, concrete wall
  G4LogicalVolume * _innerSpaceLogicalVolume; //!< the logical volume, inner space
  G4LogicalVolume * _waterPoolLogicalVolume; //!< the logical volume, water pool

  G4VPhysicalVolume * _rockWallPhysicalVolume; //!< the physical volume, rock wall
  G4VPhysicalVolume * _concreteWallPhysicalVolume; //!< the physical volume, concrete wall
  G4VPhysicalVolume * _innerSpacePhysicalVolume;   //!< the physical volume, inner space
  G4VPhysicalVolume * _waterPoolPhysicalVolume;	   //!< the physical volume, water pool

  bool _isVisible;

  bool _sensitiveWater;

  bool _countWaterFlux;
};


#endif // PANDAXIIILAB_H

