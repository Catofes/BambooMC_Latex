#include "detector/BambooMaterialFactory.hh"

#include <stdexcept>

BambooMaterialFactory * BambooMaterialFactory::_instance = 0;

BambooMaterialFactory * BambooMaterialFactory::Instance()
{
  if (_instance == 0) {
    _instance = new BambooMaterialFactory;
  }
  return _instance;
}

BambooMaterialFactory::BambooMaterialFactory()
{
}

bool BambooMaterialFactory::registerMaterial(const std::string &name, CreateMaterialCallback createCallback)
{
  return _materialMap.insert(CallbackMap::value_type(name, createCallback)).second;
}

bool BambooMaterialFactory::unregisterMaterial(const std::string &name)
{
  return _materialMap.erase(name) == 1;
}

BambooMaterial * BambooMaterialFactory::createMaterial (const std::string &name)
{
  CallbackMap::const_iterator i = _materialMap.find(name);
  if (i == _materialMap.end()) {
    throw std::runtime_error("Unknown Name for Material List!");
  }
  return (i->second)();
}
