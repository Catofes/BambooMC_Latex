#ifndef BAMBOOPHYSICSLIST_H
#define BAMBOOPHYSICSLIST_H

#include <G4VModularPhysicsList.hh>
#include <globals.hh>

class BambooPhysicsList: public G4VModularPhysicsList
{
public:

  BambooPhysicsList();

  virtual ~BambooPhysicsList();

public:

  virtual void SetCuts();

};

#endif
