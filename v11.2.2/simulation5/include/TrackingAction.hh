#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class EventAction;
class TrackingMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(EventAction*);
   ~TrackingAction() override;
   
    void  PreUserTrackingAction(const G4Track*) override;   
    void PostUserTrackingAction(const G4Track*) override;
    
    void SetParticleCount(G4bool flag) { fParticleCount = flag;};
    void SetKillNeutrons(G4bool flag)  { fKillNeutron = flag;};
    
  private:
    EventAction*        fEventAction    = nullptr;
    TrackingMessenger*  fTrackMessenger = nullptr;
        
    G4bool fParticleCount = true;
    G4bool fKillNeutron   = false;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
