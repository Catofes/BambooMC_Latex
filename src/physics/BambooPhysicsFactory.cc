#include "physics/BambooPhysicsFactory.hh"

#include <stdexcept>

BambooPhysicsFactory * BambooPhysicsFactory::_instance = 0;

BambooPhysicsFactory * BambooPhysicsFactory::Instance()
{
  if (_instance == 0) {
    _instance = new BambooPhysicsFactory;
  }
  return _instance;
}

BambooPhysicsFactory::BambooPhysicsFactory()
{
}

bool BambooPhysicsFactory::registerPhysics(const std::string &name, CreatePhysicsCallback createCallback)
{
  return _physicsMap.insert(CallbackMap::value_type(name, createCallback)).second;
}

bool BambooPhysicsFactory::unregisterPhysics(const std::string &name)
{
  return _physicsMap.erase(name) == 1;
}

BambooPhysics * BambooPhysicsFactory::createPhysics (const std::string &name)
{
  CallbackMap::const_iterator i = _physicsMap.find(name);
  if (i == _physicsMap.end()) {
    throw std::runtime_error("Unknown Name for detector part!");
  }
  return (i->second)();
}
