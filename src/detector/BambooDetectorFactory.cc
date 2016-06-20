#include "detector/BambooDetectorFactory.hh"

#include <stdexcept>

BambooDetectorFactory *BambooDetectorFactory::_instance = 0;

BambooDetectorFactory *BambooDetectorFactory::Instance()
{
    if (_instance == 0) {
        _instance = new BambooDetectorFactory;
    }
    return _instance;
}

BambooDetectorFactory::BambooDetectorFactory()
{
}

bool BambooDetectorFactory::registerDetectorPart(const std::string &type, CreateDetectorPartCallback createCallback)
{
    return _partMap.insert(CallbackMap::value_type(type, createCallback)).second;
}

bool BambooDetectorFactory::unregisterDetectorPart(const std::string &type)
{
    return _partMap.erase(type) == 1;
}

BambooDetectorPart *BambooDetectorFactory::createDetectorPart(const std::string &type, const std::string &name)
{
    CallbackMap::const_iterator i = _partMap.find(type);
    if (i == _partMap.end()) {
        throw std::runtime_error(std::string("Unknown type [ " + type + " ]  for detector part: ") + name);
    }
    return (i->second)(G4String(name));
}
