#include "ActionInitialization.hh"
#include "PrimaryGeneratorGPS.hh"
#include "RunAction.hh"
#include "EventAction.hh"
//#include "SteppingAction.hh"
//#include "StackingAction.hh"
//#include "SteppingVerbose.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  //SetUserAction(new RunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
G4int runNumber=1;
  SetUserAction(new PrimaryGeneratorGPS(fDetConstruction));
  SetUserAction(new RunAction());
  SetUserAction(new EventAction(runNumber));
  //SetUserAction(new SteppingAction());
  //SetUserAction(new StackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//G4VSteppingVerbose*
               //ActionInitialization::InitializeSteppingVerbose() const
//{
  //return new SteppingVerbose();
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
