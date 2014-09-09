#ifndef BAMBOOGLOBALVARIABLES_H
#define BAMBOOGLOBALVARIABLES_H

#include <vector>
#include <map>
#include <string>
#include <exception>

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
  map<string, string> & getParametersMap();
  
private:
  string name;
  string parentName;
  map<string, string> parameters;
  
};


class QXmlStreamReader;

class BambooGlobalVariables {

public:

  static BambooGlobalVariables * Instance();

  ~BambooGlobalVariables();

  bool loadXMLFile(const G4String & filename);

  const vector<DetectorParameters> & getDetectorPartList();

  DetectorParameters & findDetectorPartParameters(const string & name) throw (string);

private:

  BambooGlobalVariables();

  bool loadDetectorPart(QXmlStreamReader & xs);

  bool loadDetectorParameter(QXmlStreamReader & xs);

  vector<DetectorParameters> _detectorPartList;

  static BambooGlobalVariables * _instance;

  bool _readGeometry;
};

#endif
