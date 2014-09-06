#include "detector/SampleWorldBox.hh"
#include "detector/BambooDetectorFactory.hh"

// anonymous namespace to register the SampleWorldBox

namespace {

  BambooDetectorPart * createSampleWorldBox ()
  {
    return new SampleWorldBox("SampleWorldBox");
  }

  const std::string SampleWorldBoxName("SampleWorldBox");

  const bool registered = BambooDetectorFactory::Instance()->registerDetectorPart(SampleWorldBoxName, createSampleWorldBox);
}

SampleWorldBox::SampleWorldBox (const G4String & name)
  : BambooDetectorPart(name)
{
  _halfX = 100 * cm;
  _halfY = 100 * cm;
  _halfZ = 100 * cm;
}

G4bool SampleWorldBox::construct ()
{
  return true;
}

