//
// Created by herbertqiao on 6/10/16.
//

#include "detector/LatexMaterial.hh"

#include "detector/BambooMaterialFactory.hh"
#include "BambooGlobalVariables.hh"
#include "BambooUtils.hh"

#include <G4Material.hh>
#include <G4NistManager.hh>

using std::vector;

namespace
{
    BambooMaterial *createLatexMaterial()
    {
        return new LatexMaterial("LatexMaterial");
    }

    const std::string LatexMaterialName("LatexMaterial");

    const bool registered = BambooMaterialFactory::Instance()->registerMaterial(LatexMaterialName, createLatexMaterial);
}


LatexMaterial::LatexMaterial(const G4String &name)
        : BambooMaterial(name)
{
    G4cout << "Latex Material Found." << G4endl;

}

void LatexMaterial::defineMaterials()
{
    G4NistManager *pNistManager = G4NistManager::Instance();

    vector<G4Element *> elementVec;
    vector<G4Material *> materialVec;
    G4Element *H = pNistManager->FindOrBuildElement(1);
    elementVec.push_back(H);
    G4Element *C = pNistManager->FindOrBuildElement(6);
    elementVec.push_back(C);
    G4Element *N = pNistManager->FindOrBuildElement(7);
    elementVec.push_back(N);
    G4Element *O = pNistManager->FindOrBuildElement(8);
    elementVec.push_back(O);
    G4Element *F = pNistManager->FindOrBuildElement(9);
    elementVec.push_back(F);
    G4Element *Mg = pNistManager->FindOrBuildElement(12);
    elementVec.push_back(Mg);
    G4Element *Al = pNistManager->FindOrBuildElement(13);
    elementVec.push_back(Al);
    G4Element *Si = pNistManager->FindOrBuildElement(14);
    elementVec.push_back(Si);
    G4Element *P = pNistManager->FindOrBuildElement(15);
    elementVec.push_back(P);
    G4Element *S = pNistManager->FindOrBuildElement(16);
    elementVec.push_back(S);
    G4Element *Cl = pNistManager->FindOrBuildElement(17);
    elementVec.push_back(Cl);
    G4Element *Ar = pNistManager->FindOrBuildElement(18);
    elementVec.push_back(Ar);
    G4Element *K = pNistManager->FindOrBuildElement(19);
    elementVec.push_back(K);
    G4Element *Ca = pNistManager->FindOrBuildElement(20);
    elementVec.push_back(Ca);
    G4Element *Cr = pNistManager->FindOrBuildElement(24);
    elementVec.push_back(Cr);
    G4Element *Mn = pNistManager->FindOrBuildElement(25);
    elementVec.push_back(Mn);
    G4Element *Fe = pNistManager->FindOrBuildElement(26);
    elementVec.push_back(Fe);
    G4Element *Ni = pNistManager->FindOrBuildElement(28);
    elementVec.push_back(Ni);
    G4Element *Cu = pNistManager->FindOrBuildElement(29);
    elementVec.push_back(Cu);
    G4Element *Pb = pNistManager->FindOrBuildElement(82);
    elementVec.push_back(Pb);
    G4Element *Sn = pNistManager->FindOrBuildElement(50);
    elementVec.push_back(Sn);
    G4Element *Ag = pNistManager->FindOrBuildElement(47);
    elementVec.push_back(Ag);
    G4Element *Br = pNistManager->FindOrBuildElement(35);
    elementVec.push_back(Br);
    G4Element *I = pNistManager->FindOrBuildElement(53);
    elementVec.push_back(I);

    //Construct Latex
    G4Material *latex = new G4Material("LATEX", 3.828 * g / cm3, 8, kStateSolid);
    latex->AddElement(Ag, 1.817 / 3.8376);
    latex->AddElement(Br, 1.338 / 3.8376);
    latex->AddElement(I, 0.012 / 3.8376);
    latex->AddElement(C, 0.277 / 3.8376);
    latex->AddElement(H, 0.0534 / 3.8376);
    latex->AddElement(O, 0.249 / 3.8376);
    latex->AddElement(N, 0.074 / 3.8376);
    latex->AddElement(S, 0.0172 / 3.8376);
    materialVec.push_back(latex);

    G4Material *teflon = pNistManager->FindOrBuildMaterial("G4_TEFLON");
    materialVec.push_back(teflon);

    G4Material *air = pNistManager->FindOrBuildMaterial("G4_AIR");
    materialVec.push_back(air);

    //Construct vacuum
    G4Material *vacuum = new G4Material("VACCUM", 0.00120479 * 0.001 * g / cm3, 4, kStateGas);
    vacuum->AddElement(N, 0.755268);
    vacuum->AddElement(O, 0.231781);
    vacuum->AddElement(C, 0.000124);
    vacuum->AddElement(Ar, 0.012827);
    materialVec.push_back(vacuum);


    G4cout << "Available materials: " << G4endl;
    for (size_t i = 0; i < materialVec.size(); ++i) {
        G4cout << materialVec[i]->GetName();
        if (i != materialVec.size() - 1)
            G4cout << ", ";
    }
    G4cout << G4endl;
}