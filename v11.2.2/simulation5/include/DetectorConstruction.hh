#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction() override;

  public:
  
    G4VPhysicalVolume* Construct() override;

    G4Material* 
    MaterialWithSingleIsotope(G4String, G4String, G4double, G4int, G4int);
         
    void SetRadius   (G4double);              
    void SetMaterial (G4String);            

  public:  
                    
     G4double           GetRadius()     {return fRadius;};      
     G4Material*        GetMaterial()   {return fMaterial;};
                   
     void               PrintParameters();
                       
  private:
     
     G4double           fRadius = 0.;
     G4Material*        fMaterial = nullptr;
     G4LogicalVolume*   fLAbsor = nullptr;
     
     G4double           fWorldSize = 0.;
     G4Material*        fWorldMat = nullptr;
     G4VPhysicalVolume* fPWorld = nullptr;                        
     
     DetectorMessenger* fDetectorMessenger = nullptr;

  private:
    
     void               DefineMaterials();
     G4VPhysicalVolume* ConstructVolumes();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

