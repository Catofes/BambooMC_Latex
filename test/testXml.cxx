#include <CppUTest/TestHarness.h>
#include <iostream>
#include "BambooGlobalVariables.hh"

using namespace std;

TEST_GROUP(XmlGroup)
{
};

TEST(XmlGroup, FirstTest)
{
  BambooGlobalVariables * instance = BambooGlobalVariables::Instance();
  if (instance->loadXMLFile("test.xml")){
       DetectorParameters dp = instance->findDetectorPartParameters("SampleWorldBox");
       CHECK_EQUAL(dp.getDetectorPartName(), "SampleWorldBox");
       CHECK_EQUAL(100, dp.getParameterAsInt("half_x"));
  } else {
    FAIL("not a valid xml file.");
  }
  delete instance;
}
