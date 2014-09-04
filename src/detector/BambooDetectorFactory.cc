#include "detector/BambooDetectorFactory.hh"

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
