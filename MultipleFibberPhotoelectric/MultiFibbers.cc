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
#include "G4UImanager.hh"
#include "PhysicsList.hh"




#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif


//#include "G4StepLimiterPhysics.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//#include "G4ScoringManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......






int main(int argc,char** argv)
{


	if (argc<2){


		G4cout<<" "<<G4endl;
		G4cout<<"YOU MUST PROVIDE A RUN NUMBER AS ARGUMENT."<<G4endl;
		G4cout<<"Ex: ./MultipleFibbers 2 "<<G4endl;
		G4cout<<"Ex: ./MultipleFibbers run.mac 2"<<G4endl;
		G4cout<<" "<<G4endl;

		exit(0);
		}
	else

	{

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
//    G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
//    scManager->SetVerboseLevel(1);

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

G4int RunNumber;


	 if (argc>2) {
        // execute an argument macro file if exist
        RunNumber = atoi(argv[2]);
    }
    else {RunNumber = atoi(argv[1]);}


	auto actionInitialization = new ActionInitialization(detConstruction,RunNumber);

	runManager->SetUserInitialization(actionInitialization);


    //If we have Support for Visualization
#ifdef G4VIS_USE
    // Visualization manager construction
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
#endif

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc>2) {
        // execute an argument macro file if exist
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {

        //Needed with Qt sessions
        // Initialize Geant4 kernel
        runManager->Initialize();

        // start interactive session
#ifdef G4UI_USE
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
        UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
        UImanager->ApplyCommand("/control/execute init.mac");
#endif
//        if (ui->IsGUI())
//            UImanager->ApplyCommand("/control/execute gui.mac");
        ui->SessionStart();

        delete ui;
#endif
    }

    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !

#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;

    return 0;
	}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
