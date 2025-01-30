#include "RunAction.hh"
#include "Run.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>

#include "G4Threading.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <dirent.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* prim)
  : G4UserRunAction(),
    fDetector(det), fPrimary(prim), fRun(0), fHistoManager(0), outputDirectory("./")
{
 // Book predefined histograms
 fHistoManager = new HistoManager(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
 delete fHistoManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run(fDetector); 
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{    
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();
  
  // keep run condition
  if (fPrimary) { 
    G4ParticleDefinition* particle 
      = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }
             
  //histograms
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    analysisManager->OpenFile();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
  if (isMaster) fRun->EndOfRun();    
  
  //save histograms      
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->IsActive() ) {
    analysisManager->Write();
    analysisManager->CloseFile();
  }
      
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();

  CombineCSVFiles();
}

void RunAction::CombineCSVFiles() {
  std::string outputFile = outputDirectory + "combined_tracking_output.csv";

  // Open the output CSV file for writing
  std::ofstream combinedFile(outputFile, std::ios::out | std::ios::app);
  if (!combinedFile.is_open()) {
    G4cerr << "Error: Unable to open combined CSV file: " << outputFile << G4endl;
    return;
  }

  // Write the header only once (ensure all CSVs have the same header)
  combinedFile << "TrackID,ParticleName,Energy,PosX,PosY,PosZ,EventType\n";

  // Directory containing all thread-specific CSV files
  DIR *dir = opendir(outputDirectory.c_str());
  if (!dir) {
    G4cerr << "Error: Unable to open directory: " << outputDirectory << G4endl;
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(dir)) != nullptr) {
    std::string fileName = entry->d_name;

    // Only process CSV files (you can customize this if needed)
    if (fileName.find("tracking_output_thread_") != std::string::npos && fileName.find(".csv") != std::string::npos) {
      std::ifstream threadFile(outputDirectory + fileName);
      if (!threadFile.is_open()) {
        G4cerr << "Error: Unable to open file: " << fileName << G4endl;
        continue;
      }

      std::string line;
      bool firstLine = true;

      // Read each file and append its contents to the combined file
      while (std::getline(threadFile, line)) {
        // Skip the header in the thread files (except for the first file)
        if (firstLine) {
          firstLine = false;
          continue;
        }
        combinedFile << line << "\n";
      }
      threadFile.close();
    }
  }

  closedir(dir);
  combinedFile.close();

  G4cout << "CSV files combined into: " << outputFile << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
