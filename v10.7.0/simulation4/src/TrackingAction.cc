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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(EventAction* event)
:G4UserTrackingAction(), fEventAction(event)
{ }

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

  // // kill all neutrons
  // if (particle == G4Neutron::Neutron()) {
  //   fEventAction->AddEdep(energy);  
  //   G4Track* aTrack = (G4Track*)track;
  //   aTrack->SetTrackStatus(fStopAndKill);
  // }

         
  // histograms: energy at creation
  //

  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
 
  G4int ih = 0;
  const G4ParticleDefinition* particle = track->GetParticleDefinition(); 
  G4String type   = particle->GetParticleType();      
  G4double charge = particle->GetPDGCharge();
  if (charge > 3.)  ih = 11; 
  else if (particle == G4Gamma::Gamma())       ih = 4;
  else if (particle == G4Electron::Electron()) ih = 5;
  else if (particle == G4Positron::Positron()) ih = 6;
  else if (particle == G4Neutron::Neutron())   ih = 7;
  else if (particle == G4Proton::Proton())     ih = 8;
  else if (particle == G4Deuteron::Deuteron()) ih = 9;
  else if (particle == G4Alpha::Alpha())       ih = 10;       
  else if (type == "nucleus")                  ih = 11;
  else if (type == "baryon")                   ih = 12;         
  else if (type == "meson")                    ih = 13;
  else if (type == "lepton")                   ih = 14;        
  if (ih > 0) analysis->FillH1(ih,energy);
   
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
 G4ThreeVector position = track->GetPosition();
 
 fEventAction->AddEflow(energy);  
 
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
 else if (particle == G4Gamma::Gamma())       ih = 15;
 else if (particle == G4Electron::Electron()) ih = 16;
 else if (particle == G4Positron::Positron()) ih = 17;  // split these into separate parts
 else if (particle == G4Neutron::Neutron())   ih = 18;
 else if (particle == G4Proton::Proton())     ih = 19;
 else if (particle == G4Deuteron::Deuteron()) ih = 20;
 else if (particle == G4Alpha::Alpha())       ih = 21;       
 else if (type == "nucleus")                  ih = 22;
 else if (type == "baryon")                   ih = 23;         
 else if (type == "meson")                    ih = 24;
 else if (type == "lepton")                   ih = 25;        
 if (ih > 0) analysis->FillH1(ih,energy);

 if (ih > 14 && ih < 22) {
  // double x = position.x();
  // double y = position.y();
  // double z = position.z();

  // double mag = position.mag();

  // // Polar angle (theta)
  // theta = std::acos(z / mag);  // arccos(z / |v|)
  // // Azimuthal angle (phi)
  // phi = std::atan2(y, x);  // atan2(y, x)
  double theta = position.theta();
  analysis->FillH1(ih+12,std::cos(theta));

  double phi = position.phi();
  analysis->FillH1(ih+19,phi);


  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
