#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Threading.hh"
#include <TH1D.h>
#include <TFile.h>
#include <ctime>

namespace simulation2
{
G4ThreadLocal std::vector<G4double>* fThreadNeutronEnergyList = nullptr;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
RunAction::RunAction()
{
  // add new units for dose
  //
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;
  const G4double picogray  = 1.e-12*gray;

  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
  accumulableManager->RegisterAccumulable(fEdep2);

  for (size_t i = 0; i < 8; ++i) {
        fNeutronHists[i] = nullptr;
    }
}

RunAction::~RunAction() {
    // Clean up histograms
    for (size_t i = 0; i < 8; ++i) {
        if (fNeutronHists[i]) {
            delete fNeutronHists[i];
            fNeutronHists[i] = nullptr;
        }
    }
}
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  for (size_t i = 0; i < 8; ++i) {
        fNeutronHists[i] = new TH1D(Form("neutron_hist_thread_%zu", i), "Neutron Energy Distribution", 100, 0., 10.0);
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  G4double edep  = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();

  G4double rms = edep2 - edep*edep/nofEvents;
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  const auto detConstruction = static_cast<const DetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detConstruction->GetScoringVolume()->GetMass();
  G4double dose = edep/mass;
  G4double rmsDose = rms/mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
    G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  // =*= Write neutron histogram to file
  // Merge histograms from all threads
    // TH1D* mergedHist = new TH1D("neutron_hist_merged", "Merged Neutron Exit Energies", 100, 0.0, 20.0);
    
    // for (auto& entry : fNeutronHistograms) {
    //     mergedHist->Add(entry.second);  // Merge histograms from each thread
    // }

    // // Write the merged histogram to a file
    // std::string filename = "neutron_counts_" + std::to_string(std::time(nullptr)) + ".root";
    // outputFile = new TFile(filename.c_str(), "RECREATE");
    // mergedHist->Write();
    // outputFile->Close();
    // MergeNeutronEnergyLists();
    // Optionally, print the total number of neutrons collected
    // G4cout << "Total neutrons recorded: " << fNeutronEnergyList.size() << G4endl;
    
    // G4cout << "printing energies !" << G4endl;
    // TFile* file = new TFile("neutron_hist_merged.root", "RECREATE");
    // fNeutronHist->Write();
    // file->Close();

    // Print summary
    // G4cout << "End of run, total neutron count: " << fNeutronHist->GetEntries() << G4endl;
    // for (auto energy : fNeutronEnergyList) {
    //     G4cout << energy << G4endl;
    //     fNeutronHist->Fill(energy);
    // }
    
    // if (fNeutronHist) {
    //     std::string filename = "neutron_counts_" + std::to_string(std::time(nullptr)) + ".root";
    //     TFile* outputFile = new TFile(filename.c_str(), "RECREATE");

    //     fNeutronHist->Write();
    //     outputFile->Close();
    // }
    // Save each thread's histogram independently
    // for (size_t i = 0; i < 8; ++i) {
    //     if (fNeutronHists[i]) {
    //         // Check if the histogram is correctly filled before saving
    //         G4cout << "Saving histogram for thread " << i << " with " << fNeutronHists[i]->GetEntries() << " entries." << G4endl;
    //         fNeutronHists[i]->Write();
    //         if (IsMaster()) {
    //             std::string filename = Form("neutron_hist_thread_%zu.root", i);
    //             TFile* outputFile = new TFile(filename.c_str(), "RECREATE");
    //             outputFile->Close();
    //         }
    //     }
    //}
    

    // Print run summary
    G4cout << "Run summary: " << G4endl;
    G4cout << "Total events processed: " << run->GetNumberOfEvent() << G4endl;


  // Print
  //
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }

  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << " Cumulated dose per run, in scoring volume : "
     << G4BestUnit(dose,"Dose") << " rms = " << G4BestUnit(rmsDose,"Dose")
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
  fEdep2 += edep*edep;
}

void RunAction::AddNeutronEnergy(G4double energy, size_t threadId) const {
    // Add energy to the correct histogram based on threadId
    if (threadId < 8 && fNeutronHists[threadId]) {
        fNeutronHists[threadId]->Fill(energy);
    }
}

}
