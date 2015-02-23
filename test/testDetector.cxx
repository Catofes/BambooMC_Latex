#include <CppUTest/TestHarness.h>
#include "detector/BambooDetectorFactory.hh"

TEST_GROUP(DetectorGroup)
{
};

TEST(DetectorGroup, FirstTest)
{
  BambooDetectorPart * sampleBox = BambooDetectorFactory::Instance()->createDetectorPart("SampleWorldBox");
  G4String name = sampleBox->getName();
  BambooDetectorFactory::Instance()->unregisterDetectorPart("SampleWorldBox");
  delete sampleBox;
  STRCMP_EQUAL(name.c_str(), "SampleWorldBox");
}
