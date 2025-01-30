#ifndef simulation2SteppingAction_h
#define simulation2SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

#include <vector>            // =*=

class G4LogicalVolume;

namespace simulation2
{

class EventAction;

/// Stepping action class

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction* eventAction);
    ~SteppingAction() override = default;

    // method from the base class
    void UserSteppingAction(const G4Step*) override;

  private:
    EventAction* fEventAction = nullptr;
    G4LogicalVolume* fScoringVolume = nullptr;
    // std::vector<G4double> fNeutrons; // =*= Store neutron particles (make sure to define what G4Particle is or change it to the correct type)

};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
