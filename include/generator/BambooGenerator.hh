#ifndef BAMBOOGENERATOR_H
#define BAMBOOGENERATOR_H

#include <globals.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

class BambooGenerator : public G4VUserPrimaryGeneratorAction {
public:
  BambooGenerator(const G4String &name);

  virtual ~BambooGenerator() {}

  G4String getName() const { return _name; }

protected:
  G4String _name;

};
#endif
