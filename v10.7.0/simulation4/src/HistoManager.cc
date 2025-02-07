#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("Hadr06")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms
  
  // Define histograms start values
  const G4int kMaxHisto = 41;
  const G4String id[] = {"0","1","2","3","4","5","6","7","8","9",
                         "10","11","12","13","14","15","16","17","18","19",
			 "20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40"};
  const G4String title[] = 
       { "dummy",                                                        //0
         "total energy deposit",                                         //1
         "Edep (MeV/mm) along absorber",                                 //2
         "total kinetic energy flow",                                    //3
         "energy spectrum of gamma at creation",                         //4
         "energy spectrum of e- at creation",                           //5
         "energy spectrum of e+ at creation",                           //6
         "energy spectrum of neutrons at creation",                      //7
         "energy spectrum of protons at creation",                       //8
         "energy spectrum of deuterons at creation",                     //9
         "energy spectrum of alphas at creation",                        //10
         "energy spectrum of all others ions at creation",               //11
         "energy spectrum of all others baryons at creation",            //12
         "energy spectrum of all others mesons at creation",             //13
         "energy spectrum of all others leptons (neutrinos) at creation", //14
         "energy spectrum of emerging gamma",                            //15
         "energy spectrum of emerging e-",                              //16
         "energy spectrum of emerging e+",                              //17
         "energy spectrum of emerging neutrons",                         //18
         "energy spectrum of emerging protons",                          //19
         "energy spectrum of emerging deuterons",                        //20
         "energy spectrum of emerging alphas",                           //21
         "energy spectrum of all others emerging ions",                  //22
         "energy spectrum of all others emerging baryons",               //23
         "energy spectrum of all others emerging mesons",                //24
         "energy spectrum of all others emerging leptons (neutrinos)",   //25	
         "total energy released : edep + eflow",                         //26
         "theta angle of gamma",                                         //27
         "theta angle of e-",                                           //28
         "theta angle of e+",                                           //29
         "theta angle of neutrons",                                      //30
         "theta angle of protons",                                       //31
         "theta angle of deuterons",                                     //32
         "theta angle of alphas",                                        //33
         "phi angle of gamma",                                           //34
         "phi angle of e-",                                             //35
         "phi angle of e+",                                             //36
         "phi angle of neutrons",                                        //37
         "phi angle of protons",                                         //38
         "phi angle of deuterons",                                       //39
         "phi angle of alphas"                                           //40
       };

  // Default values (to be reset via /analysis/h1/set command) 
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  for (G4int k=0; k<kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
    analysisManager->SetH1Activation(ih, false);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
