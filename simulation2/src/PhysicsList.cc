#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4NuclideTable.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsXS.hh"

#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4HadronPhysicsShielding.hh"

#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonQMDPhysics.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonINCLXXPhysics.hh"

#include "GammaNuclearPhysics.hh"
#include "GammaNuclearPhysicsLEND.hh"

#include "G4RadioactiveDecayPhysics.hh"

#include "G4Gamma.hh"
#include "G4Neutron.hh"
#include "G4HadronInelasticProcess.hh"
#include "G4PhotoNuclearCrossSection.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4EmStandardPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
{
  G4int verb = 1;  
  SetVerboseLevel(verb);
  
  //add new units for cross sections
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);
    
  // mandatory for G4NuclideTable
  const G4double meanLife = 1*picosecond;  
  G4NuclideTable::GetInstance()->SetMeanLifeThreshold(meanLife);  
    
  // Hadron Elastic scattering
  // RegisterPhysics( new G4HadronElasticPhysicsHP(verb));
  RegisterPhysics( new G4HadronElasticPhysicsXS(verb));  

  // Hadron Inelastic physics
  // RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verb));
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(verb));
  // RegisterPhysics( new G4HadronPhysicsQGSP_BIC_AllHP(verb));
  // RegisterPhysics( new G4HadronPhysicsQGSP_BIC(verb));  
  // RegisterPhysics( new G4HadronInelasticQBBC(verb));
  // RegisterPhysics( new G4HadronPhysicsINCLXX(verb));
  // RegisterPhysics( new G4HadronPhysicsShielding(verb));
    
  // Ion Elastic scattering
  RegisterPhysics( new G4IonElasticPhysics(verb));
  
  // Ion Inelastic physics
  RegisterPhysics( new G4IonPhysicsXS(verb));
  // RegisterPhysics( new G4IonPhysicsPHP(verb));
  // RegisterPhysics( new G4IonQMDPhysics(verb));
  // RegisterPhysics( new G4IonINCLXXPhysics(verb));

  // Gamma physics
  RegisterPhysics( new GammaNuclearPhysics("gamma"));
  // RegisterPhysics( new GammaNuclearPhysicsLEND("gamma")); 
  
  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  
  // **Register EM standard physics**
  RegisterPhysics(new G4EmStandardPhysics()); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  SetCutValue(0.*mm, "proton");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......