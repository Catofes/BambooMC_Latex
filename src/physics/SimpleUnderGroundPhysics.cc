#include "physics/SimpleUnderGroundPhysics.hh"
#include "physics/BambooPhysicsFactory.hh"
#include "BambooGlobalVariables.hh"

#include <Shielding.hh>
#include <QString>

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

  std::string dcV = BambooGlobalVariables::Instance()->getPhysicsParameterAsString("cutlength");

  if (dcV.empty()) {
    defaultCutValue = 0.001*mm;
  } else {
    std::string::size_type n = dcV.find("*");
    double unit;
    if (n!=std::string::npos) {
      unit = BambooGlobalVariables::Instance()->getUnitByName(dcV.substr(n+1));
      defaultCutValue = QString(dcV.substr(0,n).c_str()).toDouble() * unit;
    } else {
      defaultCutValue = QString(dcV.c_str()).toDouble() * mm;
    }
  }

  G4cout << "default cut length: " << defaultCutValue/mm << " mm" << G4endl;
      
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

  // shielding, changed api for different version of geant4.
#ifdef G4VERSION_NUMBER
#if G4VERSION_NUMBER<1000
  RegisterPhysics(new HadronPhysicsShielding(verbose));
#else
  RegisterPhysics(new G4HadronPhysicsShielding(verbose));
#endif
#endif
  // stopping physics
  RegisterPhysics(new G4StoppingPhysics(verbose));

  // ion physics
  RegisterPhysics(new G4IonQMDPhysics(verbose));

}

SimpleUnderGroundPhysics::~SimpleUnderGroundPhysics() {
}
