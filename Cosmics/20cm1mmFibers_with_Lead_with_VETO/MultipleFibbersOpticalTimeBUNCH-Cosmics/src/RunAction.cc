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
// $Id: RunAction.cc 100946 2016-11-03 11:28:08Z gcosmo $
//
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(G4int runNumber, G4int acquisitionTime, G4int sourceActivity, DetectorConstruction *detector)
 : G4UserRunAction(),det(detector)
{
    RunNumber=runNumber;
    AcqTime=acquisitionTime;
    SourceActiv=sourceActivity;

    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);

    // Create analysis manager
    // The choice of analysis technology is done via selectin of a namespace
    // in Analysis.hh
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    // Create directories
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

    //Fibbers interactions
    //nTuppleID=0
    analysisManager->CreateNtuple("CosmicTreeFibbers", "CosmicTreeFibbers");
    analysisManager->CreateNtupleIColumn("runID");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleDColumn("particleEnergy");
    analysisManager->CreateNtupleDColumn("particlePosX");
    analysisManager->CreateNtupleDColumn("particlePosY");
    analysisManager->CreateNtupleDColumn("particlePosZ");
    analysisManager->CreateNtupleDColumn("particleT0_s");
    analysisManager->CreateNtupleIColumn("NaN");//let the branches stay. otherelse will be a mess to reorder the writings
    analysisManager->CreateNtupleIColumn("NaN2");
    analysisManager->CreateNtupleIColumn("NPhotonsGenerated");
    analysisManager->CreateNtupleDColumn("Ewater");
    analysisManager->CreateNtupleDColumn("Efibber");
    analysisManager->CreateNtupleDColumn("Lwater");
    analysisManager->CreateNtupleDColumn("Lfibber");
    analysisManager->CreateNtupleIColumn("nHitPMT0");
    analysisManager->CreateNtupleIColumn("nHitPMT1");
    analysisManager->CreateNtupleIColumn("nHitTotal");
    analysisManager->CreateNtupleIColumn("Coincidence");
    analysisManager->CreateNtupleIColumn("particlePDG");
    analysisManager->CreateNtupleDColumn("particleMomentumU");
    analysisManager->CreateNtupleDColumn("particleMomentumV");
    analysisManager->CreateNtupleDColumn("particleMomentumW");
    analysisManager->CreateNtupleIColumn("secondaryParticlePDG");
    analysisManager->CreateNtupleDColumn("secondaryEnergy");
    analysisManager->CreateNtupleIColumn("CoincidenceVetoFibbers");
    analysisManager->FinishNtuple();
    
    //Photon Fibers Hits
    //nTuppleID=1
    analysisManager->CreateNtuple("OpticalDataFibbers", "OtpicalDataFibbers");
    analysisManager->CreateNtupleIColumn("runID");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("photonID");
    analysisManager->CreateNtupleDColumn("HitPosX");
    analysisManager->CreateNtupleDColumn("HitPosY");
    analysisManager->CreateNtupleDColumn("HitPosZ");
    analysisManager->CreateNtupleIColumn("pmtNumber");
    analysisManager->CreateNtupleDColumn("photonEnergy");
    //analysisManager->CreateNtupleIColumn("DetectionFlag");//1 if detected 0 is not
    analysisManager->FinishNtuple();
  
    //nTuppleID=2
    analysisManager->CreateNtuple("RunConditions", "RunConditions");
    analysisManager->CreateNtupleIColumn("AcquisitionTime_s");
    analysisManager->CreateNtupleIColumn("sourceActivity_BqL");
    analysisManager->CreateNtupleDColumn("fibberLength");
    analysisManager->CreateNtupleIColumn("nFibbers");
    analysisManager->CreateNtupleDColumn("fibberDiameter");
    analysisManager->FinishNtuple();

    //Veto interactions
    //nTuppleID=3
    analysisManager->CreateNtuple("CosmicTreeVeto", "CosmicTreeVeto");
    analysisManager->CreateNtupleIColumn("runID");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleDColumn("particleEnergy");
    analysisManager->CreateNtupleDColumn("particlePosX");
    analysisManager->CreateNtupleDColumn("particlePosY");
    analysisManager->CreateNtupleDColumn("particlePosZ");
    analysisManager->CreateNtupleDColumn("particleT0_s");
    analysisManager->CreateNtupleIColumn("NaN");//let the branches stay. otherelse will be a mess to reorder the writings
    analysisManager->CreateNtupleIColumn("NaN2");
    analysisManager->CreateNtupleIColumn("NPhotonsGenerated");
    analysisManager->CreateNtupleDColumn("Ewater");
    analysisManager->CreateNtupleDColumn("Eveto");
    analysisManager->CreateNtupleDColumn("Lwater");
    analysisManager->CreateNtupleDColumn("Lveto");
    analysisManager->CreateNtupleIColumn("nHitPMT2");
    analysisManager->CreateNtupleIColumn("nHitPMT3");
    analysisManager->CreateNtupleIColumn("nHitTotalHigh");
    analysisManager->CreateNtupleIColumn("CoincidenceHigh");
    analysisManager->CreateNtupleIColumn("nHitPMT4");
    analysisManager->CreateNtupleIColumn("nHitPMT5");
    analysisManager->CreateNtupleIColumn("nHitTotalDown");
    analysisManager->CreateNtupleIColumn("CoincidenceDown");
    analysisManager->CreateNtupleIColumn("nHitTotal");
    analysisManager->CreateNtupleIColumn("CoincidenceVetos");
    analysisManager->CreateNtupleIColumn("CoincidenceVetoFibbers");
    analysisManager->CreateNtupleIColumn("particlePDG");
    analysisManager->CreateNtupleDColumn("particleMomentumU");
    analysisManager->CreateNtupleDColumn("particleMomentumV");
    analysisManager->CreateNtupleDColumn("particleMomentumW");
    analysisManager->CreateNtupleIColumn("secondaryParticlePDG");
    analysisManager->CreateNtupleDColumn("secondaryEnergy");
    analysisManager->FinishNtuple();

    //Photon Veto Hits
    //nTuppleID=4
    analysisManager->CreateNtuple("OpticalDataVeto", "OtpicalDataVeto");
    analysisManager->CreateNtupleIColumn("runID");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("photonID");
    analysisManager->CreateNtupleDColumn("HitPosX");
    analysisManager->CreateNtupleDColumn("HitPosY");
    analysisManager->CreateNtupleDColumn("HitPosZ");
    analysisManager->CreateNtupleIColumn("pmtNumber");
    analysisManager->CreateNtupleDColumn("photonEnergy");
    //analysisManager->CreateNtupleIColumn("DetectionFlag");//1 if detected 0 is not
    analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
    //G4long seeds[2];
    //G4long systime = time(NULL);
    //seeds[0] = (long) systime;
    //seeds[1] = (long) (systime*G4UniformRand());
    //G4Random::setTheSeeds(seeds);

	
    //inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Open an output file
    //
    G4cout <<"RUN NUMBER "<<RunNumber<<G4endl;
  
    G4String fileName = "MultipleFibbers_Cosmics_Run_"+std::to_string(RunNumber) ;
  
    analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
    // print histogram statistics
    //

    G4double fibberLength=det->GetFibberLength();
    G4int nFibbers=det->GetNFibbers();
    G4double fibberDiameter=2*det->GetFibberRadius();
 
 
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
 
    analysisManager->FillNtupleIColumn(2,0, AcqTime);
    analysisManager->FillNtupleIColumn(2,1, SourceActiv);
    analysisManager->FillNtupleDColumn(2,2, fibberLength);
    analysisManager->FillNtupleIColumn(2,3, nFibbers);
    analysisManager->FillNtupleDColumn(2,4, fibberDiameter);
    analysisManager->AddNtupleRow(2);

    analysisManager->Write();
    analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
