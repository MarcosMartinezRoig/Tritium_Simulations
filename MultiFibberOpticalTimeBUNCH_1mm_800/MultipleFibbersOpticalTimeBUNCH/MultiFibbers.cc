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

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "time.h"

#include "G4PhysicalConstants.hh"

#include "G4Poisson.hh"
#include "Analysis.hh"

//#include "G4StepLimiterPhysics.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void ChooseRunType(int Nevents, int activity_BqL, int acquisitionTime, DetectorConstruction* det){
    auto runManager = G4RunManager::GetRunManager();
    // Get analysis manager
    if (Nevents==0){
        //Calculate the sources volume
        G4double srcIntRadius=det->GetFibberRadius();
        G4double srcExtRadius=det->GetSourceRadius();
        G4double srcLength=2*(det->GetSourceHalfLength());
        G4double nSources=det->GetNFibbers();
			
        G4double sourcesVolume=nSources*pi*srcLength*(pow(srcExtRadius,2)-pow(srcIntRadius,2))*1e-6;//mm3 to L
			
        G4double activity=activity_BqL*sourcesVolume;
        G4cout<<"Activity "<<activity<<G4endl;
			
        //Follow the Poisson statistics for the number of events per second. Then multiplying the activity*60 i get the probabaility per minute
        G4int numberOfEvents=G4Poisson(activity*acquisitionTime);
			
        G4cout<<"numberOfEvents In Run: "<<numberOfEvents<<G4endl;
			
        runManager->BeamOn(numberOfEvents);
			
    }
    else{
        runManager->BeamOn(Nevents);
    }
}




int main(int argc,char** argv)
{
    G4int AcquisitionTime_s =0;
    G4int Activity=0;
	
    if (argc<4){

        G4cout<<" "<<G4endl;
        G4cout<<"YOU MUST PROVIDE AT LEAST 3 ARGUMENTs."<<G4endl;
        G4cout<<"Ex: ./MultipleFibbers (string)run.mac (int)runNumber (int)Nevents "<<G4endl;
        G4cout<<" OR "<<G4endl;
        G4cout<<"Ex: ./MultipleFibbers (string)run.mac (int)runNumber 0 (int)AcquisitionTime_s (int)Activity_Bq/L (int)RandomSeed"<<G4endl;
        G4cout << G4endl;
        G4cout << "The time will start to count from the run 0"<< G4endl;
        G4cout<<" "<<G4endl;
		
        exit(0);
    }
    else{
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
	

        // execute an argument macro file if exist
        G4String fileName = argv[1];
        G4int RunNumber = atoi(argv[2]);
	G4int Nevents= atoi(argv[3]);
	G4long randSeed=1;
	
	if (Nevents==0){
            G4cout<<" "<<G4endl;
            G4cout<<"Nevents=0. TimeMode Selected."<<G4endl;
            G4cout<<" "<<G4endl;
            if (argc<7){
                G4cout<<" "<<G4endl;
		G4cout<<"YOU MUST PROVIDE THE AQUISITION TIME (seconds):"<<G4endl;
		G4cout<<"Ex: ./MultipleFibbers (string)run.mac (int)runNumber 0 (int)AcquisitionTime_s (int)Activity_Bq/L (int)RandomSeed"<<G4endl;
		G4cout<<" "<<G4endl;

                exit(0);
            }
            else{
                AcquisitionTime_s = atoi(argv[4]);
                Activity = atoi(argv[5]);
                randSeed=atoi(argv[6]);
                G4cout<<"AcquisitionTime: "<<AcquisitionTime_s<< G4endl;
                G4cout <<"Source Activity: "<<Activity<<" Bq/L "<< G4endl;
            }
        }
    
    ////choose the Random engine
    //CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
    ////set random seed with system time
    //G4long seed = time(NULL);
    //CLHEP::HepRandom::setTheSeed(seed);
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4long seeds[2];
    G4long systime = time(NULL);
    seeds[0] = (long) systime;
    //seeds[1] = (long) (systime*G4UniformRand());
    seeds[1] = (long) (systime*randSeed);
    G4Random::setTheSeeds(seeds);

	
    //G4cout<<"AcquisitionTime "<<AcquisitionTime_s<<" Activity "<<Activity_Bq<<G4endl;
    auto actionInitialization = new ActionInitialization(detConstruction,RunNumber,AcquisitionTime_s,Activity);
	
    runManager->SetUserInitialization(actionInitialization);
    
    
    //If we have Support for Visualization
    // Visualization manager construction
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (fileName.contains(".mac")) {
        // execute an argument macro file if exist
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+fileName);
        
        ChooseRunType(Nevents,Activity,AcquisitionTime_s,detConstruction);
        
        //// Get analysis manager
        //auto analysisManager = G4AnalysisManager::Instance();
        //analysisManager->FillNtupleIColumn(2,5, numberOfEvents);
    }
    else {
        //Needed with Qt sessions
        // Initialize Geant4 kernel
        runManager->Initialize();
        
        // start interactive session
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        if(RunNumber==1){
        UImanager->ApplyCommand("/control/execute init_vis.mac");}
        if(RunNumber==0){
        UImanager->ApplyCommand("/control/execute init.mac");}

        ChooseRunType(Nevents,Activity,AcquisitionTime_s,detConstruction);

        if (ui->IsGUI())
            UImanager->ApplyCommand("/control/execute gui.mac");
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
