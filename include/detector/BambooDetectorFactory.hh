#ifndef BAMBOODETECTORFACTORY_H
#define BAMBOODETECTORFACTORY_H

#include <globals.hh>
#include <string>
#include <memory>
#include <stdexcept>
#include <map>

#include "BambooDetectorPart.hh"

class G4LogicalVolume;

class BambooDetectorFactory {

public:
  typedef BambooDetectorPart* (*CreateDetectorPartCallback)();

  static BambooDetectorFactory* Instance();

  ~BambooDetectorFactory();

  bool registerDetectorPart (const std::string &name, CreateDetectorPartCallback createCallback);

  bool unregisterDetectorPart (const std::string &name);

  BambooDetectorPart * createDetectorPart (const std::string &name);

private:

  BambooDetectorFactory();

  std::map<std::string, CreateDetectorPartCallback> _partMap;

  static BambooDetectorFactory * _instance;
};

#endif // BAMBOODETECTORFACTORY_H
