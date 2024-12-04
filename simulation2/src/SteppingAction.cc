#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"


#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4Neutron.hh"
#include "G4VProcess.hh"
#include "G4ProcessManager.hh"

#include "G4SystemOfUnits.hh"
#include "RunAction.hh" // Include RunAction for histogram filling =*=

namespace simulation2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    if (!fScoringVolume) {
        const auto detConstruction = static_cast<const DetectorConstruction*>(
        G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        fScoringVolume = detConstruction->GetScoringVolume();
    }
    
    // get volume of the current step
    G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    // check if we are in scoring volume
    if (volume != fScoringVolume) return;
    
    // collect energy deposited in this step
    G4double edepStep = step->GetTotalEnergyDeposit();
    fEventAction->AddEdep(edepStep);

    const G4Track* track = step->GetTrack();
    const G4ParticleDefinition* particle = track->GetParticleDefinition();

    // Check if the particle is a neutron
    // G4cout << "particle !!!! name is: " << particle->GetParticleName() << G4endl;
    
    // if (particle->GetParticleName() == "neutron") {
    //     // Get the energy of the neutron
    //     G4double energy = track->GetKineticEnergy(); // Energy in MeV
    //     // Call RunAction to store the energy in the thread-local list
    //     // G4cout << "adding energy " << energy << G4endl;
    //     // Get the current thread ID
    //     G4cout << energy << G4endl;
    //     size_t threadId = G4Threading::G4GetThreadId(); // Get the thread ID

    //     // Add neutron energy to the corresponding histogram
    //     const RunAction* runAction = dynamic_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());
    //     runAction->AddNeutronEnergy(energy, threadId);  // Add energy to the correct thread's histogram
    // }
    if (particle->GetParticleName() == "neutron") {
        // Get the process responsible for the neutron's creation
        const G4VProcess* process = step->GetPostStepPoint()->GetProcessDefinedStep();
        G4double energy = track->GetKineticEnergy(); // Energy in MeV
        // Print the name of the process for debugging
        if (process->GetProcessName() != "Transportation") {
            G4cout << energy << "\t" << process->GetProcessName() << G4endl;
        }

        // Check if the process name indicates fission
        if (process && process->GetProcessName() == "fission") {
            G4cout << "Fission neutron detected!" << G4endl;
            // You can now handle this neutron as a fission neutron
            // For example, increment counters, fill histograms, etc.
        }
    }
    // G4StepPoint* postStepPoint = step->GetPostStepPoint();
    // std::vector<const G4Track*> secondaries = step->GetSecondary();

    // for (const auto& secondary : secondaries) {
    //     G4ParticleDefinition* secondaryParticle = secondary->GetDefinition();
        
    //     // Check if secondary particle is a neutron and was produced by a fission process
    //     if (secondaryParticle == G4Neutron::Neutron()) {
    //         // You may want to check the process responsible for this secondary particle as well
    //         const G4VProcess* secondaryProcess = secondary->GetCreatorProcess();
            
    //         // Check if the process responsible for the neutron is fission
    //         if (secondaryProcess && secondaryProcess->GetProcessName() == "nFission") {
    //             G4cout << "Fission neutron (secondary) detected!" << G4endl;
    //             // Handle the fission secondary neutron: fill histograms, counters, etc.
    //         }
    //     }
    // }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
