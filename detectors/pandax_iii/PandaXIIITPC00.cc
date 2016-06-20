#include "detector/PandaXIIITPC00.hh"
#include "detector/BambooDetectorFactory.hh"
#include "BambooGlobalVariables.hh"
#include "analysis/PandaXSensitiveDetector.hh"
#include "BambooUtils.hh"

#include <G4Isotope.hh>
#include <G4Element.hh>
#include <G4Material.hh>
#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>
#include <G4Tubs.hh>
#include <G4Box.hh>
#include <G4Polycone.hh>
#include <G4Polyhedra.hh>
#include <G4UnionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4IntersectionSolid.hh>
#include <G4SDManager.hh>
#include <G4UniformElectricField.hh>
#include <G4FieldManager.hh>
#include <G4EqMagElectricField.hh>
#include <G4ClassicalRK4.hh>
#include <G4MagIntegratorStepper.hh>
#include <G4MagIntegratorDriver.hh>
#include <G4ChordFinder.hh>

#include <assert.h>

// anonymous namespace to register the PandaXIIITPC00

namespace {

    BambooDetectorPart *createPandaXIIITPC00(const G4String & name) {
      return new PandaXIIITPC00(name);
    }

    const std::string PandaXIIITPC00Name("PandaXIIITPC00");

    const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(PandaXIIITPC00Name,
                                                                                    createPandaXIIITPC00);
}

PandaXIIITPC00::PandaXIIITPC00(const G4String &name)
        : BambooDetectorPart(name) {
  //  BambooGlobalVariables *bgv = BambooGlobalVariables::Instance();

  DetectorParameters dp = BambooGlobalVariables::Instance()
          ->findDetectorPartParameters(_partName);

  _ringInnerRadius = BambooUtils::evaluate(dp.getParameterAsString("ring_inner_radius"));
  _ringThickness = BambooUtils::evaluate(dp.getParameterAsString("ring_thickness"));
  _ringHeight = BambooUtils::evaluate(dp.getParameterAsString("ring_height"));
  _ringSpacing = BambooUtils::evaluate(dp.getParameterAsString("ring_spacing"));

  _nRings = dp.getParameterAsInt("num_rings");

  _cathodeFrameHeight = BambooUtils::evaluate(dp.getParameterAsString("cathode_frame_height"));
  _cathodeHeight = BambooUtils::evaluate(dp.getParameterAsString("cathode_height"));

  _nSupportingBars = dp.getParameterAsInt("num_supporting_bars");
  _supporterWidth = BambooUtils::evaluate(dp.getParameterAsString("supporter_width"));
  _supporterThickness = BambooUtils::evaluate(dp.getParameterAsString("supporter_thickness"));
  _readoutPlateThickness = BambooUtils::evaluate(dp.getParameterAsString("readout_plate_thickness"));
  _readoutPlateKaptonThickness = BambooUtils::evaluate(dp.getParameterAsString("readout_plate_kapton_thickness"));
  int buildGas = dp.getParameterAsInt("build_sensitive_gas");

  _electricFieldDirection = dp.getParameterAsInt("electric_field_direction");

  _electricField = BambooUtils::evaluate(dp.getParameterAsString("electric_field"));

  if (buildGas == 1) {
    _buildSenstivieGas = true;
  } else {
    _buildSenstivieGas = false;
  }

  if (_ringInnerRadius <= 0) {
    _ringInnerRadius = 687 * mm;
  }
  if (_ringThickness <= 0) {
    _ringThickness = 5. * mm;
  }
  if (_ringHeight <= 0) {
    _ringHeight = 10. * mm;
  }
  if (_ringSpacing <= 0) {
    _ringSpacing = 10. * mm;
  }

  if (_cathodeFrameHeight <= 0) {
    _cathodeFrameHeight = 5. * mm;
  }
  if (_cathodeHeight <= 0) {
    _cathodeHeight = .1 * mm;
  }
  if (_nRings <= 0) {
    _nRings = 34;
  }
  if (_nSupportingBars <= 0) {
    _nSupportingBars = 24;
  }
  if (_supporterWidth <= 0) {
    _supporterWidth = 50 * mm;
  }
  if (_supporterThickness <= 0) {
    _supporterThickness = 15 * mm;
  }
  if (_readoutPlateThickness <= 0) {
    _readoutPlateThickness = 5 * mm;
  }
  if (_readoutPlateKaptonThickness <= 0) {
    _readoutPlateKaptonThickness = 0.05 * mm;
  }
  if (buildGas != 1 && buildGas != 0) {
    G4cout << "build_sensitive_gas should be set to either 0 or 1. 0 will be used as default." << G4endl;
  }

  if (_electricFieldDirection != 0 && _electricFieldDirection != 1
      && _electricFieldDirection != 2 && _electricFieldDirection != 3) {
    _electricFieldDirection = 0;
    G4cout << "The direction of the electric field can either be 0, 1, 2 and 3. 0 will be used by default." <<
    G4endl;
  }

  if (_electricField > 0) {
    _electricField = _electricField * volt / cm;
  } else {
    _electricField = 0;
  }

  G4cout << "PandaXIIITPC00 (The version 0 of traditional TPC)" << G4endl;
  G4cout << "input parameters:" << G4endl;
  G4cout << "  number of rings: " << _nRings << "." << G4endl;
  G4cout << "  number of supporting bars: " << _nSupportingBars << "." << G4endl;
}


G4bool PandaXIIITPC00::construct() {

  // create the shaping ring with polycone, two field cages are constructed
  // together. Two cathode frames will be treated as a ring (may with
  // different height). So there are 2n+1 rings

  G4Material *copper = G4Material::GetMaterial("G4_Cu");

  int nPoints = 4 * (2 * _nRings + 1);

#ifndef LARGE_ARRAY_SIZE
#define LARGE_ARRAY_SIZE 4096
#endif

  double rRing[LARGE_ARRAY_SIZE];
  double zRing[LARGE_ARRAY_SIZE];

  assert(LARGE_ARRAY_SIZE > nPoints);

  double ringsHalfZ = _nRings * (_ringHeight + _ringSpacing) + _cathodeFrameHeight;

  for (int i = 0; i < _nRings; ++i) {
    zRing[4 * i] = -ringsHalfZ + i * _ringHeight + i * _ringSpacing;
    zRing[4 * i + 1] = zRing[4 * i];
    zRing[4 * i + 2] = -ringsHalfZ + (i + 1) * _ringHeight + i * _ringSpacing;
    zRing[4 * i + 3] = zRing[4 * i + 2];
    zRing[nPoints - 4 * i - 1] = -zRing[4 * i];
    zRing[nPoints - 4 * i - 2] = -zRing[4 * i + 1];
    zRing[nPoints - 4 * i - 3] = -zRing[4 * i + 2];
    zRing[nPoints - 4 * i - 4] = -zRing[4 * i + 3];
  }
  zRing[4 * _nRings] = -_cathodeFrameHeight;
  zRing[4 * _nRings + 1] = zRing[4 * _nRings];
  zRing[4 * _nRings + 2] = _cathodeFrameHeight;
  zRing[4 * _nRings + 3] = zRing[4 * _nRings + 2];
  for (int i = 0; i < nPoints; ++i) {
    if (i % 4 == 0 || (i + 1) % 4 == 0) {
      rRing[i] = 1. * mm;
    } else {
      rRing[i] = _ringInnerRadius + _ringThickness;
    }
  }

  G4VSolid *ringsPolycone = new G4Polycone("RingsPolycone", 0, 2 * pi, nPoints, rRing, zRing);
  G4VSolid *ringsCentralTub = new G4Tubs("RingsCentralTub", 0, _ringInnerRadius, ringsHalfZ + 1. * mm, 0, 2 * pi);
  G4SubtractionSolid *ringsRealShape = new G4SubtractionSolid("RingsRealShape", ringsPolycone, ringsCentralTub);

  _ringsLog = new G4LogicalVolume(ringsRealShape, copper, "RingsLog", 0, 0, 0);
  _ringsPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _ringsLog, "ShapingRings",
                                 _parentPart->getContainerLogicalVolume(), false, 0);

  G4VisAttributes *ringsVis = new G4VisAttributes();
  ringsVis->SetColour(224. / 255, 126. / 255, 11. / 255, 0.9);
  _ringsLog->SetVisAttributes(ringsVis);

  _partLogicalVolume = _ringsLog;
  _partPhysicalVolume = _ringsPhys;
  _partContainerLogicalVolume = 0;

  // the supporting bar
  G4Material *teflon = G4Material::GetMaterial("G4_TEFLON");

  double barLength = ringsHalfZ + _ringSpacing + 5 * mm;

  double support_radius = _ringInnerRadius + _supporterThickness / 2. + 0.1 * mm;

  G4VSolid *supportPolyhedra = new G4Polyhedra("supportPolyhedra", -pi / 4, 2 * pi, 4, nPoints, rRing, zRing);
  G4VSolid *outerBox = new G4Box("OuterBox", _supporterThickness / 2, _supporterWidth / 2, barLength);

  G4ThreeVector *transport = new G4ThreeVector();
  G4RotationMatrix *rotation = new G4RotationMatrix();
  transport->setX(-support_radius * sqrt(2) / 2.);
  G4SubtractionSolid *supportingBarSolid = new G4SubtractionSolid("SupportBarSolid", outerBox, supportPolyhedra,
                                                                  G4Transform3D(*rotation, *transport));
  G4LogicalVolume *supportLog = new G4LogicalVolume(supportingBarSolid, teflon, "SupportLog", 0, 0, 0);

  // place the support in right place
  for (int i = 0; i < _nSupportingBars; ++i) {
    double angle = 2 * pi / _nSupportingBars * i;
    G4RotationMatrix *mat_r = new G4RotationMatrix();
    mat_r->rotateZ(angle);
    G4ThreeVector *loc = new G4ThreeVector();
    loc->setRhoPhiZ(support_radius, angle, 0);
    new G4PVPlacement(G4Transform3D(*mat_r, *loc), supportLog, "SupportingBar",
                      _parentPart->getContainerLogicalVolume(), false, i + 1);
  }
  G4VisAttributes *supportVis = new G4VisAttributes();
  supportVis->SetColour(224. / 255, 224. / 255, 224. / 255, 0.8);
  supportLog->SetVisAttributes(supportVis);

  G4VSolid *cathodeSolid = new G4Tubs("cathodeSolid", 0, _ringInnerRadius, _cathodeHeight / 2, 0, 2 * pi);
  _cathodeLog = new G4LogicalVolume(cathodeSolid, copper, "CathodeLog", 0, 0, 0);
  _cathodeLog->SetVisAttributes(ringsVis);
  _cathodePhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), _cathodeLog, "CathodePlate",
                                   _parentPart->getContainerLogicalVolume(), false, 0);

  G4VSolid *readoutPlateSolid = new G4Tubs("readoutPlateSolid", 0, _ringInnerRadius + _ringThickness,
                                           _readoutPlateThickness / 2, 0, 2 * pi);
  _readoutPlateLog = new G4LogicalVolume(readoutPlateSolid, copper, "ReadoutPlateLog", 0, 0, 0);
  _readoutPlateLog->SetVisAttributes(ringsVis);
  new G4PVPlacement(0, G4ThreeVector(0, 0, barLength + _readoutPlateThickness / 2), _readoutPlateLog, "ReadoutPlate",
                    _parentPart->getContainerLogicalVolume(), false, 0);
  new G4PVPlacement(0, G4ThreeVector(0, 0, -barLength - _readoutPlateThickness / 2), _readoutPlateLog, "ReadoutPlate",
                    _parentPart->getContainerLogicalVolume(), false, 1);

  G4Material *kapton = G4Material::GetMaterial("G4_KAPTON");
  G4VSolid *readoutPlateKaptonSolid = new G4Tubs("readoutPlateKaptonSolid", 0, _ringInnerRadius,
                                                 _readoutPlateKaptonThickness / 2, 0, 2 * pi);
  _readoutPlateKaptonLog = new G4LogicalVolume(readoutPlateKaptonSolid, kapton, "ReadoutPlateKaptonLog", 0, 0, 0);
  G4VisAttributes *kaptonVis = new G4VisAttributes();
  kaptonVis->SetColour(100. / 255, 100. / 255, 224. / 100, 0.8);
  _readoutPlateKaptonLog->SetVisAttributes(kaptonVis);
  new G4PVPlacement(0, G4ThreeVector(0, 0, barLength - _readoutPlateKaptonThickness / 2), _readoutPlateKaptonLog,
                    "ReadoutPlateKapton", _parentPart->getContainerLogicalVolume(), false, 0);
  new G4PVPlacement(0, G4ThreeVector(0, 0, -barLength + _readoutPlateKaptonThickness / 2), _readoutPlateKaptonLog,
                    "ReadoutPlateKapton", _parentPart->getContainerLogicalVolume(), false, 1);

  if (_buildSenstivieGas) {
    G4Material *XeTMAMixture = G4Material::GetMaterial("XeTMAMixture");
    G4VisAttributes *gasVis;
    //construct and place UpXe.
    G4Tubs *xeTub = new G4Tubs("xeTub", 0, _ringInnerRadius,
                               (barLength - _cathodeHeight / 2 - _readoutPlateKaptonThickness) / 2, 0, 2 * pi);
    _upXeLogicalVolume = new G4LogicalVolume(xeTub, XeTMAMixture, "UpXeLog");
    _downXeLogicalVolume = new G4LogicalVolume(xeTub, XeTMAMixture, "DownXeLog");
    if (_electricFieldDirection != 0) {
      G4cout << "Construct electric field in xenon..." << G4endl;
      G4ThreeVector eVec(0, 0, 0);
      switch (_electricFieldDirection) {
        case 1:
          eVec.setX(_electricField);
          break;
        case 2:
          eVec.setY(_electricField);
          break;
        case 3:
          eVec.setZ(_electricField);
          break;
      }

      // field for xenon (up)
      G4Field *electricField_up = new G4UniformElectricField(-eVec);
      G4EqMagElectricField *equation_up = new G4EqMagElectricField((G4ElectroMagneticField *) electricField_up);
      G4MagIntegratorStepper *stepper_up = new G4ClassicalRK4(equation_up, 8);
      G4FieldManager *localFieldManager_up = new G4FieldManager(electricField_up);
      G4MagInt_Driver *driver_up = new G4MagInt_Driver(0.01 * mm, stepper_up, stepper_up->GetNumberOfVariables());
      G4ChordFinder *chordFinder_up = new G4ChordFinder(driver_up);
      localFieldManager_up->SetChordFinder(chordFinder_up);
      _upXeLogicalVolume->SetFieldManager(localFieldManager_up, true);

      // field for xenon (down)
      G4Field *electricField_down = new G4UniformElectricField(eVec);
      G4EqMagElectricField *equation_down = new G4EqMagElectricField(
              (G4ElectroMagneticField *) electricField_down);
      G4MagIntegratorStepper *stepper_down = new G4ClassicalRK4(equation_down, 8);
      G4FieldManager *localFieldManager_down = new G4FieldManager(electricField_down);
      G4MagInt_Driver *driver_down = new G4MagInt_Driver(0.01 * mm, stepper_down,
                                                         stepper_down->GetNumberOfVariables());
      G4ChordFinder *chordFinder_down = new G4ChordFinder(driver_down);
      localFieldManager_down->SetChordFinder(chordFinder_down);
      _downXeLogicalVolume->SetFieldManager(localFieldManager_down, true);
    }
    // placement of xenon
    new G4PVPlacement(0, G4ThreeVector(0, 0, (barLength + _cathodeHeight / 2 - _readoutPlateKaptonThickness) / 2),
                      _upXeLogicalVolume, "ActiveXenon", _parentPart->getContainerLogicalVolume(), false,
                      0);
    new G4PVPlacement(0, G4ThreeVector(0, 0, -(barLength + _cathodeHeight / 2 - _readoutPlateKaptonThickness) / 2),
                      _downXeLogicalVolume, "ActiveXenon", _parentPart->getContainerLogicalVolume(),
                      false, 1);

    gasVis = new G4VisAttributes();
    gasVis->SetColour(0. / 255, 100. / 255, 255. / 255, 0.4);
    _upXeLogicalVolume->SetVisAttributes(gasVis);
    _downXeLogicalVolume->SetVisAttributes(gasVis);

    //set sensitive part.
    PandaXSensitiveDetector *xeSD = new PandaXSensitiveDetector("ActiveXeSD");
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(xeSD);
    _upXeLogicalVolume->SetSensitiveDetector(xeSD);
    _downXeLogicalVolume->SetSensitiveDetector(xeSD);

  }
  return true;
}
