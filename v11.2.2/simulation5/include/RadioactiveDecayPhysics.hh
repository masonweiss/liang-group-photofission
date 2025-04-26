#ifndef RadioactiveDecayPhysics_h
#define RadioactiveDecayPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RadioactiveDecayPhysics : public G4VPhysicsConstructor
{
  public: 
    RadioactiveDecayPhysics(const G4String& name = "radioactiveDecay");
   ~RadioactiveDecayPhysics() override = default;

  public:
    void ConstructParticle() override;
    void ConstructProcess()  override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif








