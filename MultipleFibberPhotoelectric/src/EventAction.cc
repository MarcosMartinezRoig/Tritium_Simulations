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
// $Id: EventAction.cc 100946 2016-11-03 11:28:08Z gcosmo $
// 
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include "PrimaryGeneratorGPS.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(G4int runNumber)
 : G4UserEventAction(),
   fEventID(0),
   fSourcePosX(0),
  fSourcePosY(0),
  fSourcePosZ(0),
   fEnergyWater(0.),
   fEnergyFibber(0.),
   fTrackLWater(0.),
   fTrackLFibber(0.),
   fSourceNumber(-1)
{RunNumber=runNumber;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{  
  //// initialisation per event
  fSourcePosX=0;
  fSourcePosY=0;
  fSourcePosZ=0;
  fEventID=0;
  fEnergyWater = 0.;
  fEnergyFibber = 0.;
  fTrackLWater = 0.;
  fTrackLFibber = 0.;
  fSourceNumber=-1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Accumulate statistics
  //

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

 fEventID = event->GetEventID();
 fPrimaryVertex = event->GetPrimaryVertex();
 
 fSourcePosX=fPrimaryVertex->GetX0();
 fSourcePosY=fPrimaryVertex->GetY0();
 fSourcePosZ=fPrimaryVertex->GetZ0();
 
 
 
 //G4double 	GetT0 () const
 G4cout<<"EVENTID "<<fEventID<<G4endl;
  //// fill ntuple
   analysisManager->FillNtupleIColumn(0, RunNumber);
  analysisManager->FillNtupleIColumn(1, fEventID);
  //analysisManager->FillNtupleDColumn(2, fParticleEnergy);->Filled in PrimaryGeneratorGPS.cc
  analysisManager->FillNtupleDColumn(3, fSourcePosX);
  analysisManager->FillNtupleDColumn(4, fSourcePosY);
  analysisManager->FillNtupleDColumn(5, fSourcePosZ);
  //analysisManager->FillNtupleIColumn(6, fSourceNumber); ->Filled in PrimaryGeneratorGPS.cc
  analysisManager->FillNtupleDColumn(7, fEnergyWater);
  analysisManager->FillNtupleDColumn(8, fEnergyFibber);
  analysisManager->FillNtupleDColumn(9, fTrackLWater);
  analysisManager->FillNtupleDColumn(10, fTrackLFibber);
  analysisManager->AddNtupleRow();  
  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
