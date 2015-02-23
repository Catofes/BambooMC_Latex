#ifndef BAMBOOANALYSISFACTORY_H
#define BAMBOOANALYSISFACTORY_H

#include <globals.hh>
#include <string>
#include <memory>
#include <stdexcept>
#include <map>

#include "BambooAnalysis.hh"

class BambooAnalysisFactory {

public:
  typedef BambooAnalysis* (*CreateAnalysisCallback)();

  static BambooAnalysisFactory* Instance();

  ~BambooAnalysisFactory();

  bool registerAnalysis (const std::string &name, CreateAnalysisCallback createCallback);

  bool unregisterAnalysis (const std::string &name);

  BambooAnalysis * createAnalysis (const std::string &name);

private:

  BambooAnalysisFactory();

  typedef std::map<std::string, CreateAnalysisCallback> CallbackMap;
  CallbackMap _analysisMap;

  static BambooAnalysisFactory * _instance;
};

#endif // BAMBOOANALYSISFACTORY_H
