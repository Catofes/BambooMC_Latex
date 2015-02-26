#include "BambooGlobalVariables.hh"

#include <QXmlStreamReader>
#include <QFile>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

BambooGlobalVariables * BambooGlobalVariables::_instance = 0;

DetectorParameters::DetectorParameters()
{
  isWorld = false;
}

DetectorParameters::~DetectorParameters()
{
  parameters.clear();
}

const string & DetectorParameters::getDetectorPartName() const
{
  return name;
}

const string & DetectorParameters::getParentName() const
{
  return parentName;
}

int DetectorParameters::getParameterAsInt(const string & parameter) const
{
  map<string, string>::const_iterator res = parameters.find(parameter);
  if (res!=parameters.end()) {
    return QString(res->second.c_str()).toInt();
  }
  return 0;
}

double DetectorParameters::getParameterAsDouble(const string & parameter) const
{
  map<string, string>::const_iterator res = parameters.find(parameter);
  if (res!=parameters.end()) {
    return QString(res->second.c_str()).toDouble();
  }
  return 0;
}

string DetectorParameters::getParameterAsString(const string & parameter) const
{
  map<string, string>::const_iterator res = parameters.find(parameter);
  if (res!=parameters.end()) {
    return res->second;
  }
  return string("");
}

map<string, string> & DetectorParameters::getParametersMap()
{
  return parameters;
}

bool DetectorParameters::isWorldPart() const
{
  return isWorld;
}

void DetectorParameters::setWorld(bool t)
{
  isWorld = t;
}

BambooGlobalVariables::~BambooGlobalVariables ()
{
  _detectorParametersList.clear();
}

BambooGlobalVariables * BambooGlobalVariables::Instance()
{
  if (_instance == 0) {
    _instance = new BambooGlobalVariables;
  }
  return _instance;
}

BambooGlobalVariables::BambooGlobalVariables ()
  : _runNumber(0), _physicsName("SimpleUnderGroundPhysics"),
    _generatorName("SimpleGPSGenerator"),
    _outDataName("pandaxout.root"),
    _readGeometry(false),
    _readDetector(false),
    _readPhysics(false)
{
}

bool BambooGlobalVariables::loadXMLFile(const G4String & filename)
{
  QFile file(QString(filename.c_str()));
  if (!file.exists()) {
    cerr << "file " << filename << " does not exist!" << endl;
    return false;
  }
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    cerr << "can't open " << filename << endl;
    return false;
  }
  QXmlStreamReader xs(&file);
  if (xs.readNextStartElement()) {
    if (xs.name()!="BambooMC") {
      cerr << "No entry for BambooMC!" << endl;
      return false;
    }
  } else {
    if (xs.hasError()) {
      cerr << "Error Code = " << xs.error() << endl;
    }
    cerr << "Can not read from file " << filename << endl;
    return false;
  }
  int nPhysics = 0;
  int nGenerator = 0;
  int nAnalysis = 0;
  _readDetector = false;
  while (!xs.atEnd()) {
    xs.readNext();
    if (xs.isStartElement()) {
      if (xs.name() == "run") {
	_runNumber = xs.attributes().value("number").toString().toInt();
      }
      if (xs.name() == "geometry") {
  	_readGeometry = true;
      }
      if (xs.name() == "detector") {
  	if (!_readGeometry)
  	  return false;
	_readDetector = true;
  	if(!loadDetectorPart(xs))
  	  return false;
      }
      if (xs.name() == "parameter") {
	if (_readDetector) {
	  if(!loadDetectorParameter(xs))
	    return false;
	} else if (_readGeometry) {
	  if (!loadGeometryParameter(xs)) {
	    return false;
	  }
	} else if (_readPhysics) {
          if (!loadPhysicsParameter(xs)) {
            return false;
          }
        } else {
          return false;
        }
      }
      if (xs.name() == "physics") {
	if (!loadPhysics(xs))
	  return false;
        _readPhysics = true;
	nPhysics++;
      }
      if (xs.name() == "generator") {
	if (!loadGenerator(xs))
	  return false;
	nGenerator++;
      }
      if (xs.name() == "analysis") {
	if (!loadAnalysis(xs))
	  return false;
	nAnalysis++;
      }
    } else if (xs.isEndElement()) {
      if (xs.name() == "geometry" ) {
	_readGeometry = false;
	cout << "finished loading geometry from xml file." << endl << endl;
      }
      if (xs.name() == "detector" ) {
	_readDetector = false;
      }
      if (xs.name() == "physics" ) {
        _readPhysics = false;
      }
    }
  }
  if (!validateDetector()) {
    return false;
  }
  if (!nPhysics>1) {
    cerr << "More than 1 physics lists are specified! (Only 1 are required!" << endl;
    return false;
  }
  if (!nGenerator>1) {
    cerr << "More than 1 generators are specified! (Only 1 are required!" << endl;
    return false;
  }
  if (!nAnalysis>1) {
    cerr << "More than 1 analysis classes are specified! (Only 1 are required!" << endl;
    return false;
  }
  return true;
}

const vector<DetectorParameters> & BambooGlobalVariables::getDetectorParametersList()
{
  return _detectorParametersList;
}

DetectorParameters & BambooGlobalVariables::findDetectorPartParameters (const string & name) throw (string)
{
  for (size_t i=0; i<_detectorParametersList.size(); ++i) {
    if (_detectorParametersList[i].getDetectorPartName()==name) {
      return _detectorParametersList[i];
    }
  }
  throw string("Unknown detector part name: ") + name;
}

BambooDetectorPart * BambooGlobalVariables::findDetectorPart(const string &name)
{
  for (size_t i=0; i<_detectorPartList.size(); ++i) {
    if (_detectorPartList[i]->getName()==name.c_str()) {
      return _detectorPartList[i];
    }
  }
  return 0;
}

void BambooGlobalVariables::addDetectorPart (BambooDetectorPart *part)
{
  _detectorPartList.push_back(part);
}

int BambooGlobalVariables::getRunNumber() const
{
  return _runNumber;
}

const string & BambooGlobalVariables::getPhysicsName ()
{
  return _physicsName;
}

const string & BambooGlobalVariables::getGeneratorName ()
{
  return _generatorName;
}

const string & BambooGlobalVariables::getAnalysisName ()
{
  return _analysisName;
}

int BambooGlobalVariables::getGeometryParameterAsInt(const string & parameter) const
{
  map<string, string>::const_iterator res = _geometryParameters.find(parameter);
  if (res!=_geometryParameters.end()) {
    return QString(res->second.c_str()).toInt();
  }
  return 0;
}

double BambooGlobalVariables::getGeometryParameterAsDouble(const string & parameter) const
{
  map<string, string>::const_iterator res = _geometryParameters.find(parameter);
  if (res!=_geometryParameters.end()) {
    return QString(res->second.c_str()).toDouble();
  }
  return 0;
}

string BambooGlobalVariables::getGeometryParameterAsString(const string & parameter) const
{
  map<string, string>::const_iterator res = _geometryParameters.find(parameter);
  if (res!=_geometryParameters.end()) {
    return res->second;
  }
  return string("");
}

int BambooGlobalVariables::getPhysicsParameterAsInt(const string & parameter) const
{
  map<string, string>::const_iterator res = _physicsParameters.find(parameter);
  if (res!=_physicsParameters.end()) {
    return QString(res->second.c_str()).toInt();
  }
  return 0;
}

double BambooGlobalVariables::getPhysicsParameterAsDouble(const string & parameter) const
{
  map<string, string>::const_iterator res = _physicsParameters.find(parameter);
  if (res!=_physicsParameters.end()) {
    return QString(res->second.c_str()).toDouble();
  }
  return 0;
}

string BambooGlobalVariables::getPhysicsParameterAsString(const string & parameter) const
{
  map<string, string>::const_iterator res = _physicsParameters.find(parameter);
  if (res!=_physicsParameters.end()) {
    return res->second;
  }
  return string("");
}

const string & BambooGlobalVariables::getOutDataName () const
{
  return _outDataName;
}

void BambooGlobalVariables::setOutDataName (const string & name)
{
  _outDataName = name;
}

void BambooGlobalVariables::setOutDataName (const char * name)
{
  _outDataName = name;
}

double BambooGlobalVariables::getUnitByName (const string & name)
{
  if (name.compare("m")==0) {
    return m;
  }
  if (name.compare("mm")==0) {
    return mm;
  }
  if (name.compare("cm")==0) {
    return cm;
  }
  return 0;
}

bool BambooGlobalVariables::loadDetectorPart(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "detector");
  DetectorParameters dp;
  dp.name = xs.attributes().value("name").toString().toStdString();
  if (!xs.attributes().value("parent").isEmpty()) {
    dp.parentName = xs.attributes().value("parent").toString().toStdString();
  }
  cout << "detector -- " << dp.name;
  if (!dp.parentName.empty())
    cout << " in " << dp.parentName;
  cout << endl;
  _detectorParametersList.push_back(dp);
  return true;
}

bool BambooGlobalVariables::loadDetectorParameter(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
  if (_detectorParametersList.size()<1)
    return false;
  DetectorParameters &dp = _detectorParametersList[_detectorParametersList.size()-1];
  string name = xs.attributes().value("name").toString().toStdString();
  string value = xs.attributes().value("value").toString().toStdString();
  dp.parameters[name] = value;
  cout << " -- parameter: " << name << " => " << value << endl;
  return true;
}

bool BambooGlobalVariables::loadGeometryParameter(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
  string name = xs.attributes().value("name").toString().toStdString();
  string value = xs.attributes().value("value").toString().toStdString();
  _geometryParameters[name] = value;
  cout << "geometry parameter: " << name << " => " << value << endl;
  return true;
}

bool BambooGlobalVariables::loadPhysics(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "physics");
  _physicsName = xs.attributes().value("name").toString().toStdString();
  cout << endl << "physics -- " << _physicsName << endl;
  return true;
}

bool BambooGlobalVariables::loadPhysicsParameter(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
  string name = xs.attributes().value("name").toString().toStdString();
  string value = xs.attributes().value("value").toString().toStdString();
  _physicsParameters[name] = value;
  cout << "physics parameter: " << name << " => " << value << endl;
  return true;
}
bool BambooGlobalVariables::loadGenerator(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "generator");
  _generatorName = xs.attributes().value("name").toString().toStdString();
  cout << endl << "generator -- " << _generatorName << endl;
  return true;
}

bool BambooGlobalVariables::loadAnalysis(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "analysis");
  _analysisName = xs.attributes().value("name").toString().toStdString();
  cout << endl << "analysis -- " << _analysisName << endl;
  return true;
}

bool BambooGlobalVariables::validateDetector()
{
  // loop over all detector parameters to see if the configuration is validate
  int nWorlds=0;
  for (size_t i=0; i<_detectorParametersList.size(); ++i) {
    if (_detectorParametersList[i].getParentName().empty()) {
      _detectorParametersList[i].setWorld(true);
      nWorlds++;
    }
  }
  if (nWorlds!=1) {
    cerr << "There are " << nWorlds << " worlds in the configuration."<< endl;
    return false;
  }
  return true;
}
