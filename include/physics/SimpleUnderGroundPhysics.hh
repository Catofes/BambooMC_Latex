#ifndef SIMPLEUNDERGROUNDPHYSICS_H
#define SIMPLEUNDERGROUNDPHYSICS_H

#include "physics/BambooPhysics.hh"

class SimpleUnderGroundPhysics : public BambooPhysics {
public:
  SimpleUnderGroundPhysics(const G4String &name);
  ~SimpleUnderGroundPhysics();
};
#endif // SIMPLEUNDERGROUNDPHYSICS
