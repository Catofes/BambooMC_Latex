#include "BambooGlobalVariables.hh"

#include <QXmlStreamReader>
#include <QFile>

BambooGlobalVariables * BambooGlobalVariables::_instance = 0;

DetectorParameters::DetectorParameters()
{
}

DetectorParameters::~DetectorParameters()
{
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

BambooGlobalVariables * BambooGlobalVariables::Instance()
{
  if (_instance == 0) {
    _instance = new BambooGlobalVariables;
  }
  return _instance;
}

BambooGlobalVariables::BambooGlobalVariables ()
{
}

bool BambooGlobalVariables::loadXMLFile(const G4String & filename)
{
  QFile file(QString(filename.c_str()));
  QXmlStreamReader xs(&file);
  while (!xs.atEnd()) {
    xs.readNext();
    // do processing
  }
  if (xs.hasError()) {
    return false;
  }
  return true;
}

const vector<DetectorParameters> BambooGlobalVariables::getDetectorPartList()
{
  return _detectorPartList;
}
