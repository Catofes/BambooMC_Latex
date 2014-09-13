#ifndef BAMBOOPHYSICS_H
#define BAMBOOPHYSICS_H

#include <globals.hh>

class BambooPhysics {
public:
  BambooPhysics(const G4String &name);

  virtual ~BambooPhysics() {}

  G4String getName() const { return _name; }

protected:
  G4String _name;

};
#endif
