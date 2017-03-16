#ifndef mscDetectorConstruction_h
#define mscDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4VPhysicalVolume;
class G4UniformMagField;
class G4GenericMessenger;

class mscDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  mscDetectorConstruction();
  virtual ~mscDetectorConstruction();
  
  virtual G4VPhysicalVolume* Construct();

  void SetNrDetectors( G4int val ){nrDet = val;}
  void SetTargetMaterial(G4String val){targetMaterial = val;}
  void SetTargetLength(G4double val){targetLen = val;}
  void UpdateGeometry();
  
private:
  // methods
  void DefineMaterials();  

  // data members
  G4int nrDet;
  G4double targetLen;
  G4String targetMaterial;
  G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

