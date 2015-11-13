#include "detector/PandaXIIIMaterial.hh"
#include "detector/BambooMaterialFactory.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"

#include <G4Material.hh>
#include <G4NistManager.hh>

#include <vector>

using std::vector;
// anonymous namespace to register the PandaXIIIMaterial

namespace {

  BambooMaterial * createPandaXIIIMaterial ()
  {
    return new PandaXIIIMaterial("PandaXIIIMaterial");
  }

  const std::string PandaXIIIMaterialName("PandaXIIIMaterial");

  const bool registered = BambooMaterialFactory::Instance()->registerMaterial(PandaXIIIMaterialName, createPandaXIIIMaterial);
}

PandaXIIIMaterial::PandaXIIIMaterial (const G4String & name)
  : BambooMaterial(name)
{
  G4cout << "PandaXIIIMaterial found..." << G4endl;

  _xe136Fraction = BambooGlobalVariables::Instance()->getMaterialParameterAsDouble("xe136_fraction");
  _enrichedXenonDensity = BambooUtils::evaluate(BambooGlobalVariables::Instance()->getMaterialParameterAsString("enriched_xenon_density"));
  _tmaMassFraction = BambooUtils::evaluate(BambooGlobalVariables::Instance()->getMaterialParameterAsString("tma_mass_fraction"));

  if (_xe136Fraction <= 0 ) {
    _xe136Fraction = 0.9;
  }
  if (_enrichedXenonDensity < 0) {
    _enrichedXenonDensity = 56.58842421 * kg/m3;
  } else {
    _enrichedXenonDensity *= kg / m3;
  }
  if (_tmaMassFraction <= 0 || _tmaMassFraction > 1) {
    _tmaMassFraction = 0.01;
  }
}


void PandaXIIIMaterial::defineMaterials()
{
  G4NistManager * pNistManager = G4NistManager::Instance();

  vector<G4Element *> elementVec;
  vector<G4Material *> materialVec;
  G4Element * H = pNistManager->FindOrBuildElement(1);
  elementVec.push_back(H);
  G4Element * C = pNistManager->FindOrBuildElement(6);
  elementVec.push_back(C);
  G4Element * N = pNistManager->FindOrBuildElement(7);
  elementVec.push_back(N);
  G4Element * O = pNistManager->FindOrBuildElement(8);
  elementVec.push_back(O);
  G4Element * F = pNistManager->FindOrBuildElement(9);
  elementVec.push_back(F);
  G4Element * Mg = pNistManager->FindOrBuildElement(12);
  elementVec.push_back(Mg);
  G4Element * Al = pNistManager->FindOrBuildElement(13);
  elementVec.push_back(Al);
  G4Element * Si = pNistManager->FindOrBuildElement(14);
  elementVec.push_back(Si);
  G4Element * P = pNistManager->FindOrBuildElement(15);
  elementVec.push_back(P);
  G4Element * S = pNistManager->FindOrBuildElement(16);
  elementVec.push_back(S);
  G4Element * Cl = pNistManager->FindOrBuildElement(17);
  elementVec.push_back(Cl);
  G4Element * K = pNistManager->FindOrBuildElement(19);
  elementVec.push_back(K);
  G4Element * Ca = pNistManager->FindOrBuildElement(20);
  elementVec.push_back(Ca);
  G4Element * Cr = pNistManager->FindOrBuildElement(24);
  elementVec.push_back(Cr);
  G4Element * Mn = pNistManager->FindOrBuildElement(25);
  elementVec.push_back(Mn);
  G4Element * Fe = pNistManager->FindOrBuildElement(26);
  elementVec.push_back(Fe);
  G4Element * Ni = pNistManager->FindOrBuildElement(28);
  elementVec.push_back(Ni);
  G4Element * Cu = pNistManager->FindOrBuildElement(29);
  elementVec.push_back(Cu);
  G4Element * Pb = pNistManager->FindOrBuildElement(82);
  elementVec.push_back(Pb);
  G4Element * Sn = pNistManager->FindOrBuildElement(50);
  elementVec.push_back(Sn);

  // enriched Xe136
  double xe128NaturalFraction = 0.019102;
  double xe129NaturalFraction = 0.264006;
  double xe130NaturalFraction = 0.04071;
  double xe131NaturalFraction = 0.212324;
  double xe132NaturalFraction = 0.269086;
  double xe134NaturalFraction = 0.104357;

  G4Isotope * Xe136 = new G4Isotope ("Xe136", 54, 136, 135.907219 * g/mole);
  G4Isotope * Xe128 = new G4Isotope ("Xe128", 54, 128, 127.9035313 * g/mole);
  G4Isotope * Xe129 = new G4Isotope ("Xe129", 54, 129, 128.9047794 * g/mole);
  G4Isotope * Xe130 = new G4Isotope ("Xe130", 54, 130, 129.9035080 * g/mole);
  G4Isotope * Xe131 = new G4Isotope ("Xe131", 54, 131, 130.9050824 * g/mole);
  G4Isotope * Xe132 = new G4Isotope ("Xe132", 54, 132, 131.9041535 * g/mole);
  G4Isotope * Xe134 = new G4Isotope ("Xe134", 54, 134, 133.9053945 * g/mole);

  G4Element * enrichedXe = new G4Element("EnrichedXe", "EXe", 7);
  enrichedXe->AddIsotope(Xe136, _xe136Fraction);
  double restFraction = 1.0 - _xe136Fraction;
  enrichedXe->AddIsotope(Xe128, restFraction * xe128NaturalFraction);
  enrichedXe->AddIsotope(Xe129, restFraction * xe129NaturalFraction);
  enrichedXe->AddIsotope(Xe130, restFraction * xe130NaturalFraction);
  enrichedXe->AddIsotope(Xe131, restFraction * xe131NaturalFraction);
  enrichedXe->AddIsotope(Xe132, restFraction * xe132NaturalFraction);
  enrichedXe->AddIsotope(Xe134, restFraction * xe134NaturalFraction);

  G4cout << "Enriched Xe Atomic Mass: " << enrichedXe->GetA()/g*mole << " g/mole." << G4endl;
  G4cout << "Xe136 mole fraction: " << _xe136Fraction << G4endl;
  G4cout << "Xe136 mass fraction: " << Xe136->GetA()*0.9/enrichedXe->GetA() << G4endl;

  elementVec.push_back(enrichedXe);

  G4cout << "Avaliable elements: " << G4endl;
  for (size_t i=0; i<elementVec.size(); ++i) {
    G4cout << elementVec[i]->GetName();
    if (i != elementVec.size()-1)
      G4cout << ", ";
  }
  G4cout << G4endl;

  double gasTemperature = 300 * kelvin;
  double xenonPressure = 10.17 * bar;
  G4Material * hpXe = new G4Material("EnrichedXe136", _enrichedXenonDensity, 1, kStateGas, gasTemperature, xenonPressure );
  hpXe->AddElement(enrichedXe, 1.0);
  materialVec.push_back(hpXe);
  G4cout << "Xenon pressure = " << xenonPressure/bar << " bar." << G4endl;
  double xenon_unit_amount = _enrichedXenonDensity*m3/(enrichedXe->GetA()*mole);
  G4cout << "Xenon unit amount " << xenon_unit_amount << " mole." << G4endl;
  
  double tmaDensity = _enrichedXenonDensity * _tmaMassFraction;
  double tma_molecule_mass = 59.1103 * g/mole;
  double tma_unit_amount = tmaDensity*m3/(tma_molecule_mass*mole);
  double tmaPressure = xenonPressure * tma_unit_amount / xenon_unit_amount;
  G4cout << "TMA pressure = " << tmaPressure / bar << " bar." << G4endl;
  G4cout << "TMA unit amount " << tma_unit_amount << " mole." <<  G4endl;
  G4cout << "TMA mole fraction " << tma_unit_amount/(tma_unit_amount+xenon_unit_amount) << G4endl;
  G4cout << "TMA density: " << tmaDensity/kg*m3 << " kg/m3" << G4endl;
  G4Material * TMA = new G4Material("Trimethylamine", tmaDensity, 3, kStateGas, gasTemperature, tmaPressure);
  TMA->AddElement(N, 1);
  TMA->AddElement(C, 3);
  TMA->AddElement(H, 9);
  materialVec.push_back(TMA);

  G4cout << "TMA molecule mass: " << TMA->GetMassOfMolecule()/g*Avogadro << " g/mol." << G4endl;

  // hpxe + tma mixture
  double xe_tma_density = _enrichedXenonDensity + tmaDensity;
  G4Material * XeTMAMixture = new G4Material("XeTMAMixture", xe_tma_density, 2, kStateGas, gasTemperature, xenonPressure+tmaPressure);
  XeTMAMixture->AddMaterial(hpXe, 1. / (1. + _tmaMassFraction));
  XeTMAMixture->AddMaterial(TMA, _tmaMassFraction / (1. + _tmaMassFraction));
  materialVec.push_back(XeTMAMixture);

  G4Material * concrete = pNistManager->FindOrBuildMaterial("G4_CONCRETE");
  materialVec.push_back(concrete);
  
  // Siguang's sim Concrete
  std::vector<G4String> els;
  els.clear();
  els.push_back("H");
  els.push_back("C");
  els.push_back("O");
  els.push_back("Na");
  els.push_back("Mg");
  els.push_back("Al");
  els.push_back("Si");
  els.push_back("P");
  els.push_back("S");
  els.push_back("K");
  els.push_back("Ca");
  els.push_back("Ti");
  els.push_back("Fe");
  std::vector<G4double> weights;
  weights.clear();
  weights.push_back(89e-4);
  weights.push_back(.0799);
  weights.push_back(.4843);
  weights.push_back(6e-3);
  weights.push_back(85e-4);
  weights.push_back(9e-3);
  weights.push_back(.0386);
  weights.push_back(4e-4);
  weights.push_back(16e-4);
  weights.push_back(54e-4);
  weights.push_back(.3506);
  weights.push_back(4e-4);
  weights.push_back(64e-4);
  
  G4Material* Sigconcrete=pNistManager->ConstructNewMaterial("Sigconcrete",els,weights,2.4*g/cm3);
  materialVec.push_back(Sigconcrete);

  G4Material * marble = new G4Material("MARBLE", 2.71*g/cm3, 7, kStateSolid);
  marble->AddElement( C, 11.88 * 0.01);
  marble->AddElement( O, 47.91 * 0.01);
  marble->AddElement(Mg, 5.58 * 0.01);
  marble->AddElement(Al, 1.535 * 0.01);
  marble->AddElement(Si, 1.27 * 0.01);
  marble->AddElement( K, 1.535 * 0.01);
  marble->AddElement(Ca, 30.29 * 0.01);
  materialVec.push_back(marble);

  G4Material * air = pNistManager->FindOrBuildMaterial("G4_AIR");
  materialVec.push_back(air);

  G4Material * water = pNistManager->FindOrBuildMaterial("G4_WATER");
  materialVec.push_back(water);

  G4Material * polyEthylene = pNistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
  materialVec.push_back(polyEthylene);

  G4Material * teflon = pNistManager->FindOrBuildMaterial("G4_TEFLON");
  materialVec.push_back(teflon);

  G4Material * copper = pNistManager->FindOrBuildMaterial("G4_Cu");
  materialVec.push_back(copper);

  G4Material * lowCarbonSteel = new G4Material("Low_Carbon_Steel", 7.85*g/cm3, 3);
  lowCarbonSteel->AddElement(Fe, 0.988);
  lowCarbonSteel->AddElement(C, 0.002);
  lowCarbonSteel->AddElement(Mn, 0.01);

  materialVec.push_back(lowCarbonSteel);

  G4cout << "Available materials: " << G4endl;
  for (size_t i=0; i<materialVec.size(); ++i) {
    G4cout << materialVec[i]->GetName();
    if (i != materialVec.size()-1)
      G4cout << ", ";
  }
  G4cout << G4endl;
}

