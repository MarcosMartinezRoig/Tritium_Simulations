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

RunAction::RunAction()
 : G4UserRunAction()
{

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

  analysisManager->CreateNtuple("TritiumTree", "TritiumTree");
  analysisManager->CreateNtupleIColumn("runID"); //Filled in PrimaryGenerator
  analysisManager->CreateNtupleIColumn("eventID"); //Filled in EventAction
  analysisManager->CreateNtupleDColumn("SourceEnergy"); //Filled in PrimaryGenerator
  analysisManager->CreateNtupleDColumn("SourcePosX"); //Filled in PrimaryGenerator
  analysisManager->CreateNtupleDColumn("SourcePosY"); //Filled in PrimaryGenerator
  analysisManager->CreateNtupleDColumn("SourcePosZ"); //Filled in PrimaryGenerator
  analysisManager->CreateNtupleDColumn("EnergyWater"); //Filled in EventAction
  analysisManager->CreateNtupleDColumn("EnergyFibber"); //Filled in EventAction
  analysisManager->CreateNtupleDColumn("TrackLWater"); //Filled in EventAction
  analysisManager->CreateNtupleDColumn("TrackLFibber"); //Filled in EventAction
  analysisManager->CreateNtupleIColumn("HitsSiPM0"); //Filled in EventAction
  analysisManager->CreateNtupleIColumn("HitsSiPM1"); //Filled in EventAction
  analysisManager->CreateNtupleIColumn("NHitsBothSiPMs"); //Filled in EventAction
  analysisManager->CreateNtupleIColumn("Coincidences"); //Filled in EventAction

  analysisManager->FinishNtuple();

/*
  analysisManager->CreateNtuple("OpticalTree", "OpticalTree");
  analysisManager->CreateNtupleIColumn("runID");
  analysisManager->CreateNtupleIColumn("eventID");
  analysisManager->CreateNtupleIColumn("trackID");
  analysisManager->CreateNtupleDColumn("PhotonPosX");
  analysisManager->CreateNtupleDColumn("PhotonPosY");
  analysisManager->CreateNtupleDColumn("PhotonPosZ");
  analysisManager->CreateNtupleIColumn("SipmNumber");
  analysisManager->CreateNtupleDColumn("PhotoEnergy");
  analysisManager->FinishNtuple();
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{


  auto analysisManager = G4AnalysisManager::Instance();


  G4String fileName = "Scintillator" ;

  analysisManager->OpenFile(fileName);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
   // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();



  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
