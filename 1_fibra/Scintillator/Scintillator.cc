#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "PhysicsList.hh"

#include "G4UImanager.hh"

#include "G4VisExecutive.hh"

#include "G4UIExecutive.hh"

#include "time.h"
#include <sys/time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    G4int RunNumber;

    G4UIExecutive *ui =0;

    if (argc<2){
        G4cout<<" "<<G4endl;
        G4cout<<"YOU MUST PROVIDE A RUN NUMBER AS ARGUMENT (0 without visualization, 1 with visualization)."<<G4endl;
        G4cout<<"Ex: ./Minimal 1 "<<G4endl;
        G4cout<<"Ex: ./Minimal run.mac 1"<<G4endl;
        G4cout<<" "<<G4endl;

        exit(0);
    }

    else{
        if (argc>2) {
            RunNumber = atoi(argv[2]);

            if(RunNumber!=0 && RunNumber!=1){
            G4cout<<"You have used an ilegal RunNumber (0 without visualization or 1 with visualization) "<<G4endl;
            exit(0);
        }
        }
        else {
            RunNumber = atoi(argv[1]);

            if(RunNumber!=0 && RunNumber!=1){
            G4cout<<"You have used an ilegal RunNumber (0 without visualization or 1 with visualization) "<<G4endl;
            exit(0);
            }

            ui = new G4UIExecutive(argc,argv);
        }

    // Construct the default run manager
   //
    #ifdef G4MULTITHREADED
    G4MTRunManager * runManager = new G4MTRunManager;
    if ( nThreads > 0 ) runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
    #else
    G4RunManager * runManager = new G4RunManager;
    #endif

    // Choose the Random engine
    //G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Seed the random number generator manually
    //G4Random::setTheSeed(myseed);

    //choose the Random engine
    //seed set to microsenconds value of time of day
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
    CLHEP::HepRandomEngine* MyRandomEngine=CLHEP::HepRandom::getTheEngine();
    timeval tim;
    gettimeofday(&tim,NULL);
    double t1=tim.tv_sec+(tim.tv_usec)*100000;
    long seedValue=(long)t1;
    MyRandomEngine->setSeed(seedValue,1);


    // Set mandatory initialization classes
    // Detector construction
    runManager-> SetUserInitialization(new DetectorConstruction());
    // Physics list
    runManager-> SetUserInitialization(new PhysicsList());
    // User action initialization
    runManager->SetUserInitialization(new ActionInitialization());

    // Initialize G4 kernel
    runManager->Initialize();

    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();


    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (!ui) {
        // Batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    if(ui)
    {
        //If we have Support for Visualization
        if(RunNumber==1){
        // start interactive session
        UImanager->ApplyCommand("/control/execute init_vis.mac");}
        if(RunNumber==0){
        UImanager->ApplyCommand("/control/execute init.mac");}

        if (ui->IsGUI())
            UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();

        delete ui;
        delete visManager;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not
    // be deleted in the main() program !
    delete runManager;

    return 0;}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
