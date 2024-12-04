#ifndef simulation2RunAction_h
#define simulation2RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

#include <TH1D.h> // =*=
#include <TFile.h> // =*=

class G4Run;

namespace simulation2
{

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    // ~RunAction() override = default;
    virtual ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

    void AddEdep (G4double edep);

    // void FillNeutronHistogram(G4double energy);  // =*=
    void AddNeutronEnergy(G4double energy, size_t threadId) const;
    // void MergeNeutronEnergyLists();
    

  private:
    G4Accumulable<G4double> fEdep = 0.;
    G4Accumulable<G4double> fEdep2 = 0.;

    // // std::map<int, TH1D*> fNeutronHistograms; // Histogram for neutron energies
    // // TFile* outputFile; // Output ROOT file

    // TFile* outputFile;  // File to store results
    // std::vector<double> fNeutronEnergyList;  // List of neutron energies for the run
    // TH1D* fNeutronHist;  // Histogram for neutron energies
    TH1D* fNeutronHists[8]; // Assuming 8 threads
    
  public:
    int numberOfBins = 100; // Number of bins for the histogram
    double minEnergy = 0; // Minimum energy for histogram
    double maxEnergy = 20.0; // Maximum energy for histogram
    
};

}

#endif

