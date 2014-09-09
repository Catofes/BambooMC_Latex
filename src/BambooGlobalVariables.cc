#include "BambooGlobalVariables.hh"

#include <QXmlStreamReader>
#include <QFile>
#include <iostream>

using std::cout;
using std::endl;

BambooGlobalVariables * BambooGlobalVariables::_instance = 0;

DetectorParameters::DetectorParameters()
{
}

DetectorParameters::~DetectorParameters()
{
  parameters.clear();
}

string DetectorParameters::getDetectorPartName()
{
  return name;
}

string DetectorParameters::getParentName()
{
  return parentName;
}

int DetectorParameters::getParameterAsInt(const string & parameter)
{
  map<string, string>::iterator res = parameters.find(parameter);
  if (res!=parameters.end()) {
    return QString(res->second.c_str()).toInt();
  }
  return 0;
}

double DetectorParameters::getParameterAsDouble(const string & parameter)
{
  map<string, string>::iterator res = parameters.find(parameter);
  if (res!=parameters.end()) {
    return QString(res->second.c_str()).toDouble();
  }
  return 0;
}

string DetectorParameters::getParameterAsString(const string & parameter)
{
  map<string, string>::iterator res = parameters.find(parameter);
  if (res!=parameters.end()) {
    return res->second;
  }
  return string("");
}

map<string, string> & DetectorParameters::getParametersMap() {
  return parameters;
}

BambooGlobalVariables::~BambooGlobalVariables ()
{
  _detectorPartList.clear();
}

BambooGlobalVariables * BambooGlobalVariables::Instance()
{
  if (_instance == 0) {
    _instance = new BambooGlobalVariables;
  }
  return _instance;
}

BambooGlobalVariables::BambooGlobalVariables ()
{
  _readGeometry = false;
}

bool BambooGlobalVariables::loadXMLFile(const G4String & filename)
{
  QFile file(QString(filename.c_str()));
  if (!file.exists()) {
    cout << "file " << filename << " does not exist!" << endl;
    return false;
  }
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    cout << "can't open " << filename << endl;
    return false;
  }
  QXmlStreamReader xs(&file);
  if (xs.readNextStartElement()) {
    if (xs.name()!="BambooMC") {
      cout << "No entry for BambooMC!" << endl;
      return false;
    }
  } else {
    if (xs.hasError()) {
      cout << "Error Code = " << xs.error() << endl;
    }
    cout << "Can not read from file " << filename << endl;
    return false;
  }
  while (!xs.atEnd()) {
    if (xs.readNextStartElement()) {
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
    }
  }
  return true;
}

const vector<DetectorParameters> & BambooGlobalVariables::getDetectorPartList()
{
  return _detectorPartList;
}

DetectorParameters & BambooGlobalVariables::findDetectorPartParameters (const string & name) throw (string)
{
  for (size_t i=0; i<_detectorPartList.size(); ++i) {
    if (_detectorPartList[i].getDetectorPartName()==name)
      return _detectorPartList[i];
  }
  throw string("Unknown detector part name: ") + name;
}

bool BambooGlobalVariables::loadDetectorPart(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "detector");
  DetectorParameters dp;
  dp.name = xs.attributes().value("name").toString().toStdString();
  if (!xs.attributes().value("parent").isEmpty()) {
    dp.parentName = xs.attributes().value("parent").string()->toStdString();
  }
  cout << "detector " << dp.name << endl;
  _detectorPartList.push_back(dp);
  return true;
}

bool BambooGlobalVariables::loadDetectorParameter(QXmlStreamReader & xs)
{
  Q_ASSERT(xs.isStartElement() && xs.name() == "parameter");
  if (_detectorPartList.size()<1)
    return false;
  DetectorParameters &dp = _detectorPartList[_detectorPartList.size()-1];
  string name = xs.attributes().value("name").toString().toStdString();
  string value = xs.attributes().value("value").toString().toStdString();
  dp.parameters[name] = value;
  cout << " -- parameter: " << name << " => " << value << endl;
  return true;
}
