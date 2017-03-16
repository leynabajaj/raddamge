#ifndef mscMessenger_HH
#define mscMessenger_HH

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4VModularPhysicsList.hh"

class mscDetectorConstruction;
class mscEventAction;
class mscPrimaryGeneratorAction;
class mscSteppingAction;

class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class mscMessenger : public G4UImessenger {
public:
  mscMessenger();
  ~mscMessenger();
  
  void SetPriGen ( mscPrimaryGeneratorAction *pg ){ fPriGen  = pg; }
  void SetDetCon ( mscDetectorConstruction *dc )  { fDetCon  = dc; }
  void SetEvAct  ( mscEventAction *ev )           { fEvAct   = ev; }
  void SetStepAct( mscSteppingAction *st )        { fStepAct = st; }
  
  void SetNewValue(G4UIcommand* cmd, G4String newValue);
  
private:
  mscDetectorConstruction *fDetCon;
  mscEventAction *fEvAct;
  mscPrimaryGeneratorAction *fPriGen;
  mscSteppingAction *fStepAct;

  G4UIcmdWithAnInteger *nrDetCmd;
  G4UIcmdWithAString *tgtMatCmd;
  G4UIcmdWithADoubleAndUnit *gunEnergyCmd;
  G4UIcmdWithAString *outNmCmd;
  G4UIcmdWithADoubleAndUnit *tgtLenCmd;
  G4UIcmdWithoutParameter *updateGeoCmd;
};

#endif//mscMessenger_HH























