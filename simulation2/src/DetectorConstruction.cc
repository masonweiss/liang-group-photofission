#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "G4Sphere.hh"

namespace simulation2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters
  //
  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_Galactic");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking

  //
  // Envelope
  //
  auto solidEnv = new G4Box("Envelope",                    // its name
    0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size

  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
    env_mat,                                     // its material
    "Envelope");                                 // its name

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),          // at (0,0,0)
    logicEnv,                 // its logical volume
    "Envelope",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Capsule shape
  G4double capsule_radius = 1*cm;
  G4ThreeVector pos2 = G4ThreeVector(0, 0, 0);
    
  G4double innerRadius = capsule_radius;    // For a solid sphere
  G4double outerRadius = 4.0 * cm; // Example outer radius
  G4double startPhi = 0.0 * deg;
  G4double endPhi = 360.0 * deg;
  G4double startTheta = 0.0 * deg;
  G4double endTheta = 180.0 * deg;
  auto solidShape2 = new G4Sphere("Shape2", innerRadius, outerRadius, startPhi, endPhi, startTheta, endTheta);
  // G4VisAttributes* visAttributes = new G4VisAttributes(G4Color(0.0, 0.0, 1.0, 0.5)); // Blue color with 50% transparency

  auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
    shape2_mat,                                        // its material
    "Shape2");                                         // its name

  // visAttributes->SetForceSolid(true); // Make it appear solid
  // logicShape2->SetVisAttributes(visAttributes);

  new G4PVPlacement(nullptr,  // no rotation
    pos2,                     // at position
    logicShape2,              // its logical volume
    "Shape2",                 // its name
    logicEnv,                 // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

  // Set Shape2 as scoring volume
  //
  fScoringVolume = logicShape2;

  // Capsule of U-238
  G4Material* uranium238 = nist->FindOrBuildMaterial("G4_U");
  G4ThreeVector pos_capsule = G4ThreeVector(0, 0, 0);

  auto solid_U238_capsule = new G4Orb("U238_Capsule", capsule_radius);  // its size

  auto logic_U238_capsule = new G4LogicalVolume(solid_U238_capsule,  // its solid
    uranium238,                                        // its material
    "U238_Capsule");                                         // its name

  new G4PVPlacement(nullptr,  // no rotation
    pos_capsule,                     // at position
    logic_U238_capsule,              // its logical volume
    "U238_Capsule",                 // its name
    logicEnv,                 // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
