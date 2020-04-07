#include "mscDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4MaterialTable.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SubtractionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4GenericMessenger.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4UserLimits.hh"
#include "G4RunManager.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

//geometry update
#include "G4LogicalBorderSurface.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mscDetectorConstruction::mscDetectorConstruction()
 : G4VUserDetectorConstruction(),
   targetLen(10*cm),
   targetMaterial("G4_W"),
   fCheckOverlaps(true)
{  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
mscDetectorConstruction::~mscDetectorConstruction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mscDetectorConstruction::UpdateGeometry()
{
    // taken from LXe example
  G4GeometryManager::GetInstance()->OpenGeometry();

  // clean up previous geometry
  G4PhysicalVolumeStore  ::GetInstance()->Clean();
  G4LogicalVolumeStore   ::GetInstance()->Clean();
  G4SolidStore           ::GetInstance()->Clean();
  G4LogicalBorderSurface ::CleanSurfaceTable();

  // define new geometry
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* mscDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  G4cout<<"Construct with target made out of "<<targetMaterial<<" of Length "<<targetLen/cm<<" cm."
	<<G4endl;

  // Get materials
  G4Material* vacuumMaterial = G4Material::GetMaterial("Galactic");
  G4Material* tgtMaterial = G4Material::GetMaterial(targetMaterial);

  if ( !tgtMaterial || !vacuumMaterial) {
    G4cerr << __PRETTY_FUNCTION__ << " Cannot retrieve materials already defined. " << G4endl;
    G4cerr << "Exiting application " << G4endl;
    exit(1);
  }  

  // World
  G4double worldSizeXYZ = 5 * m;  
  G4VSolid* worldS 
    = new G4Box("World",           // its name
                 worldSizeXYZ, worldSizeXYZ, worldSizeXYZ); // its size                         
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 vacuumMaterial,   // its material
                 "World");         // its name                                   
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 

  //Target
  G4double tgtR = 200 * cm;

  G4VSolid* tgtS 
    = new G4Tubs("targetS",           // its name
                 0, tgtR, targetLen/2., //inner R, outer R, length
		 0*deg,360*deg);
  G4LogicalVolume* tgtL
    = new G4LogicalVolume(
                 tgtS,           // its solid
                 tgtMaterial,  // its material
                 "targetL");         // its name
  G4Colour  blue(0/255.,0/255.,255/255.);
  G4VisAttributes* tgtVisAtt = new G4VisAttributes(blue);
  tgtVisAtt->SetForceSolid(true);
  tgtVisAtt->SetVisibility(true);
  tgtL->SetVisAttributes(tgtVisAtt);
  new G4PVPlacement(0,G4ThreeVector(0,0,targetLen/2.),
                 tgtL,             // its logical volume                         
                 "target",         // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 

  //
  //Detectors
  //
  G4double detectorThickness = 0.01 * mm;

  G4VSolid* detUS_S 
    = new G4Tubs("detUS_S",           // its name
                 0, tgtR, detectorThickness, //inner R, outer R, length
		 0*deg,360*deg);
  G4LogicalVolume* detUS_L
    = new G4LogicalVolume(
                 detUS_S,           // its solid
                 vacuumMaterial,  // its material
                 "detUS_L");         // its name
  G4Colour  red(255/255.,0/255.,0/255.);
  G4VisAttributes* detVisAtt = new G4VisAttributes(red);
  detVisAtt->SetForceSolid(true);
  detVisAtt->SetVisibility(true);
  detUS_L->SetVisAttributes(detVisAtt);
  new G4PVPlacement(0,G4ThreeVector(0,0,-0.5*cm),
                 detUS_L,             // its logical volume                         
                 "detUS_PV",         // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 


  G4VSolid* detDS_S 
    = new G4Tubs("detDS_S",           // its name
                 0, tgtR, detectorThickness, //inner R, outer R, length
		 0*deg,360*deg);
  G4LogicalVolume* detDS_L
    = new G4LogicalVolume(
                 detDS_S,           // its solid
                 vacuumMaterial,  // its material
                 "detDS_L");         // its name
  detDS_L->SetVisAttributes(detVisAtt);
  new G4PVPlacement(0,G4ThreeVector(0,0,targetLen + 0.5*cm),
                 detDS_L,             // its logical volume                         
                 "detDS_PV",         // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void mscDetectorConstruction::DefineMaterials()
{ 

  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_H");
  nistManager->FindOrBuildMaterial("G4_C");
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_W");
  nistManager->FindOrBuildMaterial("G4_Al");
  nistManager->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  nistManager->FindOrBuildMaterial("G4_CONCRETE");
  nistManager->FindOrBuildMaterial("G4_Fe");
  nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");

  G4Element* H  = new G4Element("Hydrogen","H",1, 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon","C", 6, 12.01*g/mole);
  G4Element* B  = new G4Element("Boron","B",5,10.811*g/mole);
  // // Vacuum
  new G4Material("Galactic", 1., 1.01*g/mole, universe_mean_density,
		 kStateGas, 2.73*kelvin, 3.e-18*pascal);

  G4Material * Borated_Polythene = new G4Material("Borated_Polythene", 1.19*g/cm3,3);
  Borated_Polythene->AddElement(B,0.300);
  Borated_Polythene->AddElement(H,0.1006);
  Borated_Polythene->AddElement(C,0.5994);
  // Print materials
  G4cout << G4endl << G4endl << "~~~~~~~~~~~~~~~~~~~~~Material Printout~~~~~~~~~~~~~~~~~~~~~~~~" << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl << G4endl;
}

