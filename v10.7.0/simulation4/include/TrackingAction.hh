#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"
#include <fstream>

#include "G4Track.hh"
#include "EventAction.hh"  // Include EventAction header for inter-class communication
#include <fstream>

class EventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(EventAction*);
   ~TrackingAction();
   
    virtual void  PreUserTrackingAction(const G4Track*);   
    virtual void PostUserTrackingAction(const G4Track*);
    
  private:
    EventAction* fEventAction;
    std::ofstream fCSVFile;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
