#include "generator/BambooGeneratorFactory.hh"

#include <stdexcept>

BambooGeneratorFactory * BambooGeneratorFactory::_instance = 0;

BambooGeneratorFactory * BambooGeneratorFactory::Instance()
{
  if (_instance == 0) {
    _instance = new BambooGeneratorFactory;
  }
  return _instance;
}

BambooGeneratorFactory::BambooGeneratorFactory()
{
}

bool BambooGeneratorFactory::registerGenerator(const std::string &name, CreateGeneratorCallback createCallback)
{
  return _generatorMap.insert(CallbackMap::value_type(name, createCallback)).second;
}

bool BambooGeneratorFactory::unregisterGenerator(const std::string &name)
{
  return _generatorMap.erase(name) == 1;
}

BambooGenerator * BambooGeneratorFactory::createGenerator (const std::string &name)
{
  CallbackMap::const_iterator i = _generatorMap.find(name);
  if (i == _generatorMap.end()) {
    throw std::runtime_error("Unknown Name for detector part!");
  }
  return (i->second)();
}
