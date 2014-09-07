#ifndef BAMBOOGLOBALVARIABLES_H
#define BAMBOOGLOBALVARIABLES_H

#include <vector>
#include <map>
#include <string>

#include <globals.hh>
#include <G4String.hh>

using std::vector;
using std::map;
using std::string;

class BambooGlobalVariables;

class DetectorParameters {
  friend class BambooGlobalVariables;

public:
  DetectorParameters();
  ~DetectorParameters();
  string getDetectorPartName();
  string getParentName();
  int getParameterAsInt(const string & parameter);
  double getParameterAsDouble(const string & parameter);
  string getParameterAsString(const string & parameter);
  
private:
  string name;
  string parentName;
  map<string, string> parameters;
  
};

class BambooGlobalVariables {

public:

  static BambooGlobalVariables * Instance();

  ~BambooGlobalVariables();

  bool loadXMLFile(const G4String & filename);

  const vector<DetectorParameters> getDetectorPartList();

private:

  BambooGlobalVariables();

  vector<DetectorParameters> _detectorPartList;

  static BambooGlobalVariables * _instance;
};

#endif
