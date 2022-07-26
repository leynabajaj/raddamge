#include "mscPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mscPrimaryGeneratorAction::mscPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0),
   gunEnergy(200.)
{
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    //= G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    //= G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    //= G4ParticleTable::GetParticleTable()->FindParticle("neutron");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  G4cout<<"Particle gun constructor:: Default fun energy:"<<gunEnergy<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

mscPrimaryGeneratorAction::~mscPrimaryGeneratorAction()
{
  G4cout<<"Particle gun constructor:: Default gun energy:"<<gunEnergy<<G4endl;
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void mscPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{  

  // Set gun position
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -0.25*cm));

  fParticleGun->SetParticleEnergy(gunEnergy);
  
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

