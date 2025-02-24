//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file TrackingAction.cc
/// \brief Implementation of the TrackingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TrackingAction.hh"

#include "Run.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "TrackingMessenger.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4ParticleTypes.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(EventAction* event)
: fEventAction(event)
{   
  fTrackMessenger = new TrackingMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{
  delete fTrackMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{  
  //count secondary particles
  if (track->GetTrackID() == 1) return;
    
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
	
  const G4ParticleDefinition* particle = track->GetParticleDefinition();
  G4String name     = particle->GetParticleName();
  G4double meanLife = particle->GetPDGLifeTime();
  G4double energy   = track->GetKineticEnergy();
  if (fParticleCount) run->ParticleCount(name,energy,meanLife);
       
  // histograms: energy at creation
  //
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
 
  G4int ih = 0;
  G4String type   = particle->GetParticleType();      
  G4double charge = particle->GetPDGCharge();
  if (particle == G4Gamma::Gamma())            ih = 1;
  else if (particle == G4Electron::Electron()) ih = 2;
  else if (particle == G4Positron::Positron()) ih = 3;
  else if (particle == G4Neutron::Neutron())   ih = 4;
  // else if (particle == G4Proton::Proton())     ih = 8;
  // else if (particle == G4Deuteron::Deuteron()) ih = 9;
  // else if (particle == G4Alpha::Alpha())       ih = 10;       
  // else if (type == "nucleus")                  ih = 11;
  // else if (type == "baryon")                   ih = 12;         
  // else if (type == "meson")                    ih = 13;
  // else if (type == "lepton")                   ih = 14;  
  if (ih > 0) analysis->FillH1(ih,energy); // FIX THIS BACK TO NORMAL
   
  //to force only 1 fission : kill secondary neutrons
  if (fKillNeutron && (particle == G4Neutron::Neutron())) {
    // fEventAction->AddEdep(energy);  
    G4Track* aTrack = (G4Track*)track;
    aTrack->SetTrackStatus(fStopAndKill);
  }
  // first check process
  const G4VProcess* creatorProcess = track->GetCreatorProcess();
  if (creatorProcess) {
    G4String processName = creatorProcess->GetProcessName();

    // neutron products of possible fission reactions
    if (particle == G4Neutron::Neutron()) {
      // photonuclear reaction neutrons
      if (processName == "photonNuclear") {
        analysis->FillH1(9, energy); // Fill histogram
      }
      // neutron fission neutrons
      else if (processName == "nFission") {
        analysis->FillH1(10, energy); // Fill histogram
      }
    }

    // check it is a decay product
    if (type == "nucleus") {
      if (energy > 1) {  // only look at decay products (have high energy)
        G4String processName = creatorProcess->GetProcessName();
        G4String particleName = particle->GetParticleName();
        G4int b_val = particle->GetBaryonNumber();
        
        if (processName == "photonNuclear" && b_val > 4) {
          analysis->FillH1(11, b_val); // nucleon number
          analysis->FillH1(13, energy); // energy
        }
        // neutron fission decay products
        else if (processName == "nFission" && b_val > 4) {
          analysis->FillH1(12, b_val); // nucleon number
          analysis->FillH1(14, energy); // energy
        }
        // G4cout << particleName << " of energy " << energy << "and mass" << nucnum << "created by process: " << processName << G4endl;
      }
    }
    // pair production
    if (processName == "conv" && (particle == G4Electron::Electron() || particle == G4Positron::Positron())) {
      analysis->FillH1(15, energy); // energy
    }
    // compton scattering e-
    if (processName == "compt" && particle == G4Electron::Electron()) {
      analysis->FillH1(16, energy); // energy
    }
    // compton scattering gamma
    if (processName == "conv" && particle == G4Electron::Electron()) {
      analysis->FillH1(17, energy); // energy
    }

    // // all photo-nuclear resultant particles
    // if (processName == "photonNuclear") {
    //   analysis->FillH1(45, energy); // Fill histogram
    // }
    // // all neutron fission resultant particles
    // else if (processName == "nFission") {
    //   analysis->FillH1(47, energy); // Fill histogram
    // }
  }
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
  
  // fEventAction->AddEflow(energy);  
  
  Run* run = static_cast<Run*>(
                G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->ParticleFlux(name,energy);               
  
  // histograms: energy at exit
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
  
  G4int ih = 0; 
  G4String type   = particle->GetParticleType();      
  G4double charge = particle->GetPDGCharge();
  if (particle == G4Gamma::Gamma())       ih = 5;
  else if (particle == G4Electron::Electron()) ih = 6;
  else if (particle == G4Positron::Positron()) ih = 7;
  else if (particle == G4Neutron::Neutron())   ih = 8;
  // else if (particle == G4Proton::Proton())     ih = 19;
  // else if (particle == G4Deuteron::Deuteron()) ih = 20;
  // else if (particle == G4Alpha::Alpha())       ih = 21;       
  // else if (type == "nucleus")                  ih = 22;
  // else if (type == "baryon")                   ih = 23;         
  // else if (type == "meson")                    ih = 24;
  // else if (type == "lepton")                   ih = 25;        
  if (ih > 0) analysis->FillH1(ih,energy);

  // if (ih > 14 && ih < 22) {
  //   // double x = position.x();
  //   // double y = position.y();
  //   // double z = position.z();

  //   // double mag = position.mag();

  //   // // Polar angle (theta)
  //   // theta = std::acos(z / mag);  // arccos(z / |v|)
  //   // // Azimuthal angle (phi)
  //   // phi = std::atan2(y, x);  // atan2(y, x)
  //   // double theta = position.theta();
  //   // analysis->FillH1(ih+12,std::cos(theta));

  //   // double phi = position.phi();
  //   // analysis->FillH1(ih+19,phi);
  // }
  // // analysis->FillH1(52, energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

