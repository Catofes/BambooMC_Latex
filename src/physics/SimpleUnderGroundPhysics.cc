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
  G4int ver = 1;

  // EM Physics
  RegisterPhysics(new G4EmStandardPhysics(ver));

  // hadron physics
  RegisterPhysics(new HadronPhysicsShielding(ver));
}

SimpleUnderGroundPhysics::~SimpleUnderGroundPhysics() {
}
