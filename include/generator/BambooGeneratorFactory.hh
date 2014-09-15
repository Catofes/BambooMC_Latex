#ifndef BAMBOOGENERATORFACTORY_H
#define BAMBOOGENERATORFACTORY_H

#include <globals.hh>
#include <string>
#include <memory>
#include <stdexcept>
#include <map>

#include "BambooGenerator.hh"

class BambooGeneratorFactory {

public:
  typedef BambooGenerator* (*CreateGeneratorCallback)();

  static BambooGeneratorFactory* Instance();

  ~BambooGeneratorFactory();

  bool registerGenerator (const std::string &name, CreateGeneratorCallback createCallback);

  bool unregisterGenerator (const std::string &name);

  BambooGenerator * createGenerator (const std::string &name);

private:

  BambooGeneratorFactory();

  typedef std::map<std::string, CreateGeneratorCallback> CallbackMap;
  CallbackMap _generatorMap;

  static BambooGeneratorFactory * _instance;
};

#endif // BAMBOOGENERATORFACTORY_H
