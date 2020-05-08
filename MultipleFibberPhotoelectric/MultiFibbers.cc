//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: example.cc 70284 2013-05-28 17:26:43Z perl $
//
/// \file example.cc
/// \brief Main program of the analysis/ example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4EmStandardPhysics.hh"
#include "PhysicsList.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

//#include "G4StepLimiterPhysics.hh"

#include "G4VisExecutive.hh"

#include "G4UIExecutive.hh"


//#include "G4ScoringManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......






int main(int argc,char** argv)
{
    G4int RunNumber;

    G4UIExecutive *ui =0;

    if (argc<2){
        G4cout<<" "<<G4endl;
        G4cout<<"YOU MUST PROVIDE A RUN NUMBER AS ARGUMENT (0 without visualization, 1 with visualization)."<<G4endl;
        G4cout<<"Ex: ./MultipleFibbers 1 "<<G4endl;
        G4cout<<"Ex: ./MultipleFibbers run.mac 1"<<G4endl;
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
    // Note that if we have built G4 with support for Multi-threading we set it here
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    //Set the default number of threads to be the number of available cores of the machine
    //If not specified use 2 threads
    runManager->SetNumberOfThreads( G4Threading::G4GetNumberOfCores() );
#else
    G4RunManager* runManager = new G4RunManager;
#endif


    // Activate UI-command base scorer
    //G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
    //scManager->SetVerboseLevel(1);

    // Mandatory user initialization classes

    //====================
    //The Geometry
    DetectorConstruction* detConstruction = new DetectorConstruction;
	runManager->SetUserInitialization(detConstruction);


    //====================
    //The Physics

     runManager->SetUserInitialization(new PhysicsList());


    //====================
    // User action initialization
    auto actionInitialization = new ActionInitialization(detConstruction,RunNumber);
    runManager->SetUserInitialization(actionInitialization);

    //Needed with Qt sessions
    // Initialize Geant4 kernel
    runManager->Initialize();

    // Visualization manager construction
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();



    if(!ui){
        // execute an argument macro file if exist
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

        //if (ui->IsGUI())
        //UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();

        delete ui;
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    delete visManager;

    delete runManager;

    return 0;
	}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
