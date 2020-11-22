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
   targetLen(20*cm),
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
  G4Material* tgtMaterial1 = G4Material::GetMaterial("G4_CONCRETE");

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
/*
  G4VSolid* tgtS1 
    = new G4Tubs("targetS1",           // its name
                 0, tgtR, 5*cm, //inner R, outer R, length
		 0*deg,360*deg);
  G4LogicalVolume* tgtL1
    = new G4LogicalVolume(
                 tgtS1,           // its solid
                 tgtMaterial1,  // its material
                 "targetL1");         // its name
  G4Colour  green(0/255.,255/255.,0/255.);
  G4VisAttributes* tgtVisAtt1 = new G4VisAttributes(green);
  tgtVisAtt1->SetForceSolid(true);
  tgtVisAtt1->SetVisibility(true);
  tgtL1->SetVisAttributes(tgtVisAtt1);
  new G4PVPlacement(0,G4ThreeVector(0,0,-5*cm),
                 tgtL1,             // its logical volume                         
                 "target1",         // its name
                 worldLV,          // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
  
		 */
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

  G4double weightRatio(0);
  G4double density(0);
  G4int nComponents(0);
  G4Material *lightConcrete = new G4Material("LightConcrete", density=1.526*g/cm3, nComponents=15);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Al"), weightRatio=0.0715247535321842);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Ba"), weightRatio=0.000292615925692978);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Ca"), weightRatio=0.0902578428842246);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Fe"), weightRatio=0.0443403751565724);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_H") , weightRatio=0.0126910373153661);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_K") , weightRatio=0.0181825600330761);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_O") , weightRatio=0.0117754923569376);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_P") , weightRatio=0.000961479923817951);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_S") , weightRatio=0.0136690903190142);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Mg"), weightRatio=0.502123887700617);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Mn"), weightRatio=0.000484778323047815);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Na"), weightRatio=0.0028364585260099);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Si"), weightRatio=0.227263709531935);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Sr"), weightRatio=0.000110504797259485);
  lightConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_Ti"), weightRatio=0.00348541367424452);

  
  G4Material *bor5crete = new G4Material("Borated5Concrete", density= 2.42*g/cm3, nComponents=2);
  bor5crete->AddMaterial(nistManager->FindOrBuildMaterial("G4_B"), 5.*perCent);
  bor5crete->AddMaterial(nistManager->FindOrBuildMaterial("G4_CONCRETE"), 95.*perCent);

  G4Material *Na2O = new G4Material("Na2O",density=2.27*g/cm3,nComponents=2);
  Na2O->AddElement(nistManager->FindOrBuildElement("Na"),2);
  Na2O->AddElement(nistManager->FindOrBuildElement("O"),1);

  G4Material *P2O5 = new G4Material("P2O5",density=2.39*g/cm3,nComponents=2);
  P2O5->AddElement(nistManager->FindOrBuildElement("P"),2);
  P2O5->AddElement(nistManager->FindOrBuildElement("O"),5);

  G4Material *SO2 = new G4Material("SO2",density=2.63*g/cm3,nComponents=2);
  SO2->AddElement(nistManager->FindOrBuildElement("S"),1);
  SO2->AddElement(nistManager->FindOrBuildElement("O"),2);

  G4Material *NaCl = new G4Material("NaCl",density=2.16*g/cm3,nComponents=2);
  NaCl->AddElement(nistManager->FindOrBuildElement("Na"),1);
  NaCl->AddElement(nistManager->FindOrBuildElement("Cl"),1);

  G4Material *MnO2 = new G4Material("MnO2",density=5.03*g/cm3,nComponents=2);
  MnO2->AddElement(nistManager->FindOrBuildElement("Mn"),1);
  MnO2->AddElement(nistManager->FindOrBuildElement("O"),2);

  G4Material *NiO = new G4Material("NiO",density=6.67*g/cm3,nComponents=2);
  NiO->AddElement(nistManager->FindOrBuildElement("Ni"),1);
  NiO->AddElement(nistManager->FindOrBuildElement("O"),1);

  G4Material *cement = new G4Material("cement",density=3.13*g/cm3,nComponents=7);
  cement->AddMaterial(nistManager->FindOrBuildMaterial("G4_CALCIUM_OXIDE"),weightRatio=0.60486);
  cement->AddMaterial(nistManager->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE"),weightRatio=0.03530);
  cement->AddMaterial(nistManager->FindOrBuildMaterial("G4_POTASSIUM_OXIDE"),weightRatio=0.00109);
  cement->AddMaterial(nistManager->FindOrBuildMaterial("G4_FERRIC_OXIDE"),weightRatio=0.03778);
  cement->AddMaterial(nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE"),weightRatio=0.22370);
  cement->AddMaterial(nistManager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE"),weightRatio=0.07050);
  cement->AddMaterial(SO2,weightRatio=0.02677);

  G4Material *barite = new G4Material("barite",density=4.05*g/cm3,nComponents=16);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_CALCIUM_OXIDE"),weightRatio=0.04800);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_MAGNESIUM_OXIDE"),weightRatio=0.00649);
  barite->AddMaterial(Na2O,weightRatio=0.00027);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_POTASSIUM_OXIDE"),weightRatio=0.00024);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_FERRIC_OXIDE"),weightRatio=0.00229);
  barite->AddMaterial(P2O5,weightRatio=0.00019);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE"),weightRatio=0.03293);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_WATER"),weightRatio=0.03913);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE"),weightRatio=0.01366);
  barite->AddMaterial(SO2,weightRatio=0.00205);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_BARIUM_SULFATE"),weightRatio=0.83132);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_MAGNESIUM_CARBONATE"),weightRatio=0.00090);
  barite->AddMaterial(NaCl,weightRatio=0.00090);
  barite->AddMaterial(nistManager->FindOrBuildMaterial("G4_CALCIUM_CARBONATE"),weightRatio=0.01803);
  barite->AddMaterial(MnO2,weightRatio=0.00180);
  barite->AddMaterial(NiO,weightRatio=0.00180);


  G4Material *BariteConcrete = new G4Material("BariteConcrete",density=3.36*g/cm3, nComponents=3);
  BariteConcrete->AddMaterial(cement,weightRatio=0.0829);
  BariteConcrete->AddMaterial(nistManager->FindOrBuildMaterial("G4_WATER"),weightRatio=0.0593);
  BariteConcrete->AddMaterial(barite,weightRatio=0.8578);

  // Print materials
  G4cout << G4endl << G4endl << "~~~~~~~~~~~~~~~~~~~~~Material Printout~~~~~~~~~~~~~~~~~~~~~~~~" << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl << G4endl;
}

