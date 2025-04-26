#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:
    EventAction() = default;
   ~EventAction() override = default;

  public:
    void BeginOfEventAction(const G4Event*) override;
    void   EndOfEventAction(const G4Event*) override;
    
    void AddEdep (G4double Edep);
    void AddEflow(G4double Eflow);          
                
  private:
    G4double fTotalEnergyDeposit = 0.;
    G4double fTotalEnergyFlow = 0.;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
