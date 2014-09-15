#include "physics/SimpleUnderGroundPhysics.hh"
#include "physics/BambooPhysicsFactory.hh"

#include <Shielding.hh>
//#include <HadronPhysicsShielding.hh>

// anonymous namespace to register the SimpleUnderGroundPhysics
namespace {
  BambooPhysics * createSimpleUnderGroundPhysics ()
  {
    return new SimpleUnderGroundPhysics("SimpleUnderGroundPhysics");
  }

  const std::string SimpleUnderGroundPhysicsName("SimpleUnderGroundPhysics");

  const bool registered = BambooPhysicsFactory::Instance()->registerPhysics(SimpleUnderGroundPhysicsName, createSimpleUnderGroundPhysics);
}


SimpleUnderGroundPhysics::SimpleUnderGroundPhysics(const G4String &name) : BambooPhysics(name) {

  G4DataQuestionaire it(photon, neutron, radioactive);

  G4cout << "Physics List: " << _name << G4endl;
  defaultCutValue = 0.7*mm;
  G4int verbose = 1;

  // EM Physics
  RegisterPhysics(new G4EmStandardPhysics(verbose));

    // Synchroton Radiation & GN Physics
  RegisterPhysics( new G4EmExtraPhysics(verbose) );

  // Decays 
  RegisterPhysics( new G4DecayPhysics(verbose) );


  RegisterPhysics( new G4RadioactiveDecayPhysics(verbose) );

  // hadron physics
  RegisterPhysics(new G4HadronElasticPhysicsHP(verbose));
  RegisterPhysics(new HadronPhysicsShielding(verbose));

  // stopping physics
  RegisterPhysics(new G4StoppingPhysics(verbose));

  // ion physics
  RegisterPhysics(new G4IonQMDPhysics(verbose));
}

SimpleUnderGroundPhysics::~SimpleUnderGroundPhysics() {
}
