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
  : _physicsName("SimpleUnderGroundPhysics"),
    _generatorName("SimpleGPSGenerator"), _readGeometry(false)
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
  while (!xs.atEnd()) {
    xs.readNext();
    if (xs.isStartElement()) {
      if (xs.name() == "geometry") {
  	_readGeometry = true;
      }
      if (xs.name() == "detector") {
  	if (!_readGeometry)
  	  return false;
  	if(!loadDetectorPart(xs))
  	  return false;
      }
      if (xs.name() == "parameter") {
  	if (!_readGeometry)
  	  return false;
  	if(!loadDetectorParameter(xs))
  	  return false;
      }
      if (xs.name() == "physics") {
	if (!loadPhysics(xs))
	  return false;
	nPhysics++;
      }
      if (xs.name() == "generator") {
	if (!loadGenerator(xs))
	  return false;
	nGenerator++;
      }
    } else if (xs.isEndElement()) {
      if (xs.name() == "geometry" ) {
	_readGeometry = false;
	cout << "finished loading geometry from xml file." << endl << endl;
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

const string & BambooGlobalVariables::getPhysicsName ()
{
  return _physicsName;
}

const string & BambooGlobalVariables::getGeneratorName ()
{
  return _generatorName;
}

bool BambooGlobalVariables::loadDetectorPart(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "detector");
  DetectorParameters dp;
  dp.name = xs.attributes().value("name").toString().toStdString();
  if (!xs.attributes().value("parent").isEmpty()) {
    dp.parentName = xs.attributes().value("parent").string()->toStdString();
  }
  cout << "detector -- " << dp.name << endl;
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

bool BambooGlobalVariables::loadPhysics(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "physics");
  _physicsName = xs.attributes().value("name").toString().toStdString();
  cout << "physics -- " << _physicsName << endl << endl;
  return true;
}

bool BambooGlobalVariables::loadGenerator(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "physics");
  _generatorName = xs.attributes().value("name").toString().toStdString();
  cout << "generator -- " << _generatorName << endl << endl;
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
