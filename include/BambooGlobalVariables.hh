#ifndef BAMBOOGLOBALVARIABLES_H
#define BAMBOOGLOBALVARIABLES_H

#include <vector>
#include <map>
#include <string>
#include <exception>

#include <globals.hh>
#include <G4String.hh>

#include "detector/BambooDetectorPart.hh"

using std::vector;
using std::map;
using std::string;

class BambooGlobalVariables;

class DetectorParameters {
  friend class BambooGlobalVariables;

public:
  DetectorParameters();
  ~DetectorParameters();
  const string & getDetectorPartName() const;
  const string & getParentName() const;
  int getParameterAsInt(const string & parameter) const;
  double getParameterAsDouble(const string & parameter) const;
  string getParameterAsString(const string & parameter) const;
  map<string, string> & getParametersMap();
  bool isWorldPart() const;
  void setWorld(bool t);

private:
  string name;
  string parentName;
  bool isWorld;
  map<string, string> parameters; 
};


class QXmlStreamReader;

class BambooGlobalVariables {

public:

  static BambooGlobalVariables * Instance();

  ~BambooGlobalVariables();

  bool loadXMLFile(const G4String & filename);

  const vector<DetectorParameters> & getDetectorParametersList();

  DetectorParameters & findDetectorPartParameters(const string & name) throw (string);

  BambooDetectorPart * findDetectorPart(const string &name);

  void addDetectorPart(BambooDetectorPart * part);

  int getRunNumber() const;

  const string & getPhysicsName();

  const string & getGeneratorName();

  const string & getAnalysisName();

  int getGeometryParameterAsInt(const string & parameter) const;

  double getGeometryParameterAsDouble(const string & parameter) const;

  string getGeometryParameterAsString(const string & parameter) const;

  int getPhysicsParameterAsInt(const string & parameter) const;

  double getPhysicsParameterAsDouble(const string & parameter) const;

  string getPhysicsParameterAsString(const string & parameter) const;

  const string & getOutDataName () const;

  void setOutDataName(const string &name);

  void setOutDataName(const char * name);

private:

  BambooGlobalVariables();

  bool loadDetectorPart(QXmlStreamReader & xs);

  bool loadDetectorParameter(QXmlStreamReader & xs);

  bool loadGeometryParameter(QXmlStreamReader & xs);

  bool loadPhysics(QXmlStreamReader & xs);

  bool loadPhysicsParameter(QXmlStreamReader & xs);

  bool loadGenerator(QXmlStreamReader & xs);

  bool loadAnalysis(QXmlStreamReader & xs);

  bool validateDetector();

  int _runNumber;

  vector<DetectorParameters> _detectorParametersList;

  vector<BambooDetectorPart *> _detectorPartList;

  map<string, string> _geometryParameters;

  map<string, string> _physicsParameters;

  static BambooGlobalVariables * _instance;

  string _physicsName;

  string _generatorName;

  string _analysisName;

  string _outDataName;

  bool _readGeometry;

  bool _readDetector;

  bool _readPhysics;
};

#endif
