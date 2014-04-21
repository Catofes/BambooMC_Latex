#include "BambooPhysicsList.hh"

#include <G4OpticalPhysics.hh>
#include <G4OpticalProcessIndex.hh>

#include <G4SystemOfUnits.hh>


BambooPhysicsList::BambooPhysicsList() : G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  defaultCutValue = 1.0*mm;
  RegisterPhysics(new G4OpticalPhysics());
}

BambooPhysicsList::~BambooPhysicsList() {}

void BambooPhysicsList::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
}
