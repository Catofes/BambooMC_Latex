#ifndef BAMBOOPHYSICSFACTORY_H
#define BAMBOOPHYSICSFACTORY_H

#include <globals.hh>
#include <string>
#include <memory>
#include <stdexcept>
#include <map>

#include "BambooPhysics.hh"

class BambooPhysicsFactory {

public:
  typedef BambooPhysics* (*CreatePhysicsCallback)();

  static BambooPhysicsFactory* Instance();

  ~BambooPhysicsFactory();

  bool registerPhysics (const std::string &name, CreatePhysicsCallback createCallback);

  bool unregisterPhysics (const std::string &name);

  BambooPhysics * createPhysics (const std::string &name);

private:

  BambooPhysicsFactory();

  typedef std::map<std::string, CreatePhysicsCallback> CallbackMap;
  CallbackMap _physicsMap;

  static BambooPhysicsFactory * _instance;
};

#endif // BAMBOOPHYSICSFACTORY_H
