#include "TrackingAction.hh"

#include "Run.hh"
#include "EventAction.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTypes.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <fstream>
#include <sstream>
#include <thread>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(EventAction* event)
:G4UserTrackingAction(), fEventAction(event), fCSVFile()
{ 
  // Open a unique CSV file per thread based on thread ID
    std::stringstream filename;
    filename << "tracking_output_thread_" << std::this_thread::get_id() << ".csv";
    fCSVFile.open(filename.str(), std::ios::out | std::ios::app);
    if (!fCSVFile.is_open()) {
        G4cerr << "Error: Cannot open CSV file for thread " << std::this_thread::get_id() << G4endl;
    }

    // Write the header if the file is empty
    if (fCSVFile.tellp() == 0) {
        fCSVFile << "TrackID,ParticleName,Energy,PosX,PosY,PosZ,EventType\n";
    }
}

TrackingAction::~TrackingAction()
{
    if (fCSVFile.is_open()) {
        fCSVFile.close();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{  
  //count secondary particles
  if (track->GetTrackID() == 1) return;  
  G4String name   = track->GetDefinition()->GetParticleName();
  G4double energy = track->GetKineticEnergy();
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());    
  run->ParticleCount(name,energy);
       
  // histograms: energy at creation
  //
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
 
  G4int ih = 0;
  const G4ParticleDefinition* particle = track->GetParticleDefinition(); 
  G4String type   = particle->GetParticleType();      
  G4double charge = particle->GetPDGCharge();
  if (charge > 3.)  ih = 10; 
  else if (particle == G4Gamma::Gamma())       ih = 4;
  else if (particle == G4Electron::Electron()) ih = 5;
  else if (particle == G4Positron::Positron()) ih = 5;  
  else if (particle == G4Neutron::Neutron())   ih = 6;
  else if (particle == G4Proton::Proton())     ih = 7;
  else if (particle == G4Deuteron::Deuteron()) ih = 8;
  else if (particle == G4Alpha::Alpha())       ih = 9;       
  else if (type == "nucleus")                  ih = 10;
  else if (type == "baryon")                   ih = 11;         
  else if (type == "meson")                    ih = 12;
  else if (type == "lepton")                   ih = 13;        
  if (ih > 0) analysis->FillH1(ih,energy);

  // Log data when the particle is created (at the beginning of the track)
    if (track->GetParentID() == 0) {  // Only log primary particles
        G4int trackID = track->GetTrackID();
        G4String particleName = track->GetDefinition()->GetParticleName();
        G4double energy = track->GetKineticEnergy();
        G4ThreeVector position = track->GetPosition();

        // Log information with "Creation" event type
        fCSVFile << trackID << ","
                 << particleName << ","
                 << energy << ","
                 << position.x() << ","
                 << position.y() << ","
                 << position.z() << ","
                 << "Creation\n";
    }
   
  //to force only 1 fission : kill secondary neutrons
  ///if (particle == G4Neutron::Neutron()) {
  ///  fEventAction->AddEdep(energy);  
  ///  G4Track* aTrack = (G4Track*)track;
  ///  aTrack->SetTrackStatus(fStopAndKill);
  ///}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
 // keep only emerging particles
 G4StepStatus status = track->GetStep()->GetPostStepPoint()->GetStepStatus();
 if (status != fWorldBoundary) return; 
 
 const G4ParticleDefinition* particle = track->GetParticleDefinition();
 G4String name   = particle->GetParticleName();
 G4double energy = track->GetKineticEnergy();
 
 fEventAction->AddEflow(energy);  

 if (track->GetParentID() == 0) {  // Only log primary particles when detected
        fCSVFile << track->GetTrackID() << ","
                 << name << ","
                 << energy << ","
                 << position.x() << ","
                 << position.y() << ","
                 << position.z() << ","
                 << "Detection\n";  // Log "Detection" event type
    }
 
 Run* run = static_cast<Run*>(
              G4RunManager::GetRunManager()->GetNonConstCurrentRun());
 run->ParticleFlux(name,energy);               
 
 // histograms: energy at exit
 //
 G4AnalysisManager* analysis = G4AnalysisManager::Instance();
 
 G4int ih = 0; 
 G4String type   = particle->GetParticleType();      
 G4double charge = particle->GetPDGCharge();
 if (charge > 3.)  ih = 20; 
 else if (particle == G4Gamma::Gamma())       ih = 14;
 else if (particle == G4Electron::Electron()) ih = 15;
 else if (particle == G4Positron::Positron()) ih = 15;  
 else if (particle == G4Neutron::Neutron())   ih = 16;
 else if (particle == G4Proton::Proton())     ih = 17;
 else if (particle == G4Deuteron::Deuteron()) ih = 18;
 else if (particle == G4Alpha::Alpha())       ih = 19;       
 else if (type == "nucleus")                  ih = 20;
 else if (type == "baryon")                   ih = 21;         
 else if (type == "meson")                    ih = 22;
 else if (type == "lepton")                   ih = 23;        
 if (ih > 0) analysis->FillH1(ih,energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

