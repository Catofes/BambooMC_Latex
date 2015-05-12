#include "detector/BambooDetectorFactory.hh"

#include <stdexcept>

BambooDetectorFactory * BambooDetectorFactory::_instance = 0;

BambooDetectorFactory * BambooDetectorFactory::Instance()
{
  if (_instance == 0) {
    _instance = new BambooDetectorFactory;
  }
  return _instance;
}

BambooDetectorFactory::BambooDetectorFactory()
{
}

bool BambooDetectorFactory::registerDetectorPart(const std::string &name, CreateDetectorPartCallback createCallback)
{
  return _partMap.insert(CallbackMap::value_type(name, createCallback)).second;
}

bool BambooDetectorFactory::unregisterDetectorPart(const std::string &name)
{
  return _partMap.erase(name) == 1;
}

BambooDetectorPart * BambooDetectorFactory::createDetectorPart (const std::string &name)
{
  CallbackMap::const_iterator i = _partMap.find(name);
  if (i == _partMap.end()) {
    throw std::runtime_error(std::string("Unknown Name for detector part: ") + name);
  }
  return (i->second)();
}
