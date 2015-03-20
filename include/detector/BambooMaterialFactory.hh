#ifndef BAMBOOMATERIALFACTORY_H
#define BAMBOOMATERIALFACTORY_H

#include <globals.hh>
#include <string>
#include <memory>
#include <stdexcept>
#include <map>

#include "BambooMaterial.hh"

class BambooMaterialFactory {

public:
  typedef BambooMaterial* (*CreateMaterialCallback)();

  static BambooMaterialFactory* Instance();

  ~BambooMaterialFactory();

  bool registerMaterial (const std::string &name, CreateMaterialCallback createCallback);

  bool unregisterMaterial (const std::string &name);

  BambooMaterial * createMaterial (const std::string &name);

private:

  BambooMaterialFactory();

  typedef std::map<std::string, CreateMaterialCallback> CallbackMap;
  CallbackMap _materialMap;

  static BambooMaterialFactory * _instance;
};

#endif // BAMBOOMATERIALFACTORY_H
