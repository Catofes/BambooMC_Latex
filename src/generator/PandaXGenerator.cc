#include "generator/PandaXGenerator.hh"
#include "generator/BambooGeneratorFactory.hh"

#include <G4Event.hh>
#include <G4GeneralParticleSource.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>

// anonymous namespace to register the PandaXGenerator
namespace {
  BambooGenerator * createPandaXGenerator ()
  {
    return new PandaXGenerator("PandaXGenerator");
  }

  const std::string PandaXGeneratorName("PandaXGenerator");

  const bool registered = BambooGeneratorFactory::Instance()->registerGenerator(PandaXGeneratorName, createPandaXGenerator);
}

PandaXGenerator::PandaXGenerator(const G4String &name)
  : BambooGenerator(name)
{
  _particleGun = new G4GeneralParticleSource();
  _pandaxParticleSource = new PandaXParticleSource();
}

PandaXGenerator::~PandaXGenerator()
{
  delete _particleGun;
  delete _pandaxParticleSource;
}

void PandaXGenerator::GeneratePrimaries(G4Event * anEvent)
{
  _pandaxParticleSource->GeneratePrimaryVertex(anEvent);
  //  _particleGun->GeneratePrimaryVertex(anEvent);
}
