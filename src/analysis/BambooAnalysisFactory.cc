#include "analysis/BambooAnalysisFactory.hh"

#include <stdexcept>

BambooAnalysisFactory * BambooAnalysisFactory::_instance = 0;

BambooAnalysisFactory * BambooAnalysisFactory::Instance()
{
  if (_instance == 0) {
    _instance = new BambooAnalysisFactory;
  }
  return _instance;
}

BambooAnalysisFactory::BambooAnalysisFactory()
{
}

bool BambooAnalysisFactory::registerAnalysis(const std::string &name, CreateAnalysisCallback createCallback)
{
  return _analysisMap.insert(CallbackMap::value_type(name, createCallback)).second;
}

bool BambooAnalysisFactory::unregisterAnalysis(const std::string &name)
{
  return _analysisMap.erase(name) == 1;
}

BambooAnalysis * BambooAnalysisFactory::createAnalysis (const std::string &name)
{
  CallbackMap::const_iterator i = _analysisMap.find(name);
  if (i == _analysisMap.end()) {
    throw std::runtime_error("Unknown Name for detector part!");
  }
  return (i->second)();
}
