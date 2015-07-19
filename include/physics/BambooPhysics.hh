#ifndef BAMBOOPHYSICS_H
#define BAMBOOPHYSICS_H

#include <globals.hh>
#include <G4Version.hh>
#include <G4VModularPhysicsList.hh>

class BambooPhysics : public G4VModularPhysicsList {
public:
  BambooPhysics(const G4String &name);

  virtual ~BambooPhysics() {}

  G4String getName() const { return _name; }

protected:
  G4String _name;

};
#endif
