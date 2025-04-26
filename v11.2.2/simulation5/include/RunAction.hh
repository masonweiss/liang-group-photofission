#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction;
class Run;
class PrimaryGeneratorAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*);
   ~RunAction() override;

  public:
    G4Run* GenerateRun() override;  
    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;
                            
  private:
    DetectorConstruction*      fDetector     = nullptr;
    PrimaryGeneratorAction*    fPrimary      = nullptr;
    Run*                       fRun          = nullptr;    
    HistoManager*              fHistoManager = nullptr;
        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

