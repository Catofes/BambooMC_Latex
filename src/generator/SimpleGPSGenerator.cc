#include "generator/SimpleGPSGenerator.hh"
#include "generator/BambooGeneratorFactory.hh"

#include <G4Event.hh>
#include <G4GeneralParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>

// anonymous namespace to register the SimpleGPSGenerator
namespace {
  BambooGenerator * createSimpleGPSGenerator ()
  {
    return new SimpleGPSGenerator("SimpleGPSGenerator");
  }

  const std::string SimpleGPSGeneratorName("SimpleGPSGenerator");

  const bool registered = BambooGeneratorFactory::Instance()->registerGenerator(SimpleGPSGeneratorName, createSimpleGPSGenerator);
}

SimpleGPSGenerator::SimpleGPSGenerator(const G4String &name)
  : BambooGenerator(name)
{
  _particleGun = new G4GeneralParticleSource();
}

SimpleGPSGenerator::~SimpleGPSGenerator()
{
  delete _particleGun;
}

void SimpleGPSGenerator::GeneratePrimaries(G4Event * anEvent)
{
  _particleGun->GeneratePrimaryVertex(anEvent);
}
