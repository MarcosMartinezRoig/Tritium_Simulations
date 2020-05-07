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
#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
//#include "StackingAction.hh"
#include "sipmSD.hh"
#include "sipmHit.hh"
#include "Randomize.hh"
#include <iomanip>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(G4int runNumber)
 : G4UserEventAction(),
   fEventID(0),
   fEnergyWater(0.),
   fEnergyFibber(0.),
   fTrackLWater(0.),
   fTrackLFibber(0.),
   fSourceNumber(-1),
   fCoincidenceFlag(-1)
{RunNumber=runNumber;
	hitsCollID = -1;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
  //// initialisation per event
   fEventID=0;
  fEnergyWater = 0.;
  fEnergyFibber = 0.;
  fTrackLWater = 0.;
  fTrackLFibber = 0.;
  fSourceNumber=-1;
  fCoincidenceFlag=-1;


  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  if(hitsCollID<0){

    G4String colNam;

    hitsCollID = SDman->GetCollectionID(colNam="HitsCollectionSiPMs");

  }



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Accumulate statistics
  //

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

 fEventID = event->GetEventID();
 G4cout<<"EVENTID "<<fEventID<<G4endl;


  if(hitsCollID<0) return;


  G4HCofThisEvent * HCE = event->GetHCofThisEvent();

  sipmHitsCollection* HC = 0;
 nHitsipm0=0;
nHitsipm1=0;
//nDetectedsipm0=0;
//nDetectedsipm1=0;


  if(HCE)

  {

    HC = (sipmHitsCollection*)(HCE->GetHC(hitsCollID));

  }



  if ( HC ) {
    int n_hit = HC->entries();

    G4cout << "HITS!!!!!!! " << n_hit << G4endl;

    for (G4int i=0; i<n_hit; i++){

      sipmNumber  = (*HC)[i]->GetsipmNb();

      G4cout << "Number of SiPM: " << sipmNumber << G4endl;

      switch (sipmNumber){

		  case 0: nHitsipm0=nHitsipm0+1;
					break;

		  case 1: nHitsipm1=nHitsipm1+1;
					break;

		  }


      position = (*HC)[i]->GetPos();

      G4cout << "Position x" << position.x() << G4endl;
      G4cout << "Position y" << position.y() << G4endl;
      G4cout << "Position z" << position.z() << G4endl;


	trackID=(*HC)[i]->GetTrackID();
	G4cout << "trackID" << trackID << G4endl;


	photonEnergy=(*HC)[i]->GetEnergy();
	G4cout << "Energy" << photonEnergy << G4endl;

	//fDetection=(*HC)[i]->GetDetection();

        //FillTreeOptical(analysisManager);


    }

  }



 if(nHitsipm0 > 0 && nHitsipm1 >0){fCoincidenceFlag=1;}
 else {fCoincidenceFlag=0;}


 FillTreeTritium(analysisManager);

}
 //G4double 	GetT0 () const


void EventAction::FillTreeTritium(G4AnalysisManager *analysisManager){


  //// fill ntuples
  //analysisManager->FillNtupleIColumn(0,0, RunNumber);
  analysisManager->FillNtupleIColumn(0,1, fEventID);
  //analysisManager->FillNtupleDColumn(0,2, fParticleEnergy);->Filled in PrimaryGeneratorGPS.cc
  //analysisManager->FillNtupleDColumn(0,3, fSourcePosX);->Filled in PrimaryGeneratorGPS.cc
  //analysisManager->FillNtupleDColumn(0,4, fSourcePosY);->Filled in PrimaryGeneratorGPS.cc
  //analysisManager->FillNtupleDColumn(0,5, fSourcePosZ);->Filled in PrimaryGeneratorGPS.cc
  //analysisManager->FillNtupleDColumn(0,6, fSourceT0);->Filled in PrimaryGeneratorGPS.cc
  //analysisManager->FillNtupleIColumn(0,7, fSourceNumber); ->Filled in PrimaryGeneratorGPS.cc
   //analysisManager->FillNtupleIColumn(0,8, distanceToFibber); ->Filled in PrimaryGeneratorGPS.cc
   //analysisManager->FillNtupleIColumn(0,9, fPhotonCounter);->Filled in StackingAction.cc
  //analysisManager->FillNtupleDColumn(0,6, fEnergyWater);
  //analysisManager->FillNtupleDColumn(0,7, fEnergyFibber);
  //analysisManager->FillNtupleDColumn(0,8, fTrackLWater);
  //analysisManager->FillNtupleDColumn(0,9, fTrackLFibber);
  analysisManager->FillNtupleIColumn(0,10, nHitsipm0);
  analysisManager->FillNtupleIColumn(0,11, nHitsipm1);
  analysisManager->FillNtupleIColumn(0,12, nHitsipm0+nHitsipm1);
  analysisManager->FillNtupleIColumn(0,13, fCoincidenceFlag);
  //analysisManager->FillNtupleIColumn(0,17, nDetectedsipm0);
  //analysisManager->FillNtupleIColumn(0,18, nDetectedsipm1);
  //analysisManager->FillNtupleIColumn(0,19, nDetectedsipm0+nDetectedsipm1);

  analysisManager->AddNtupleRow(0);


}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::FillTreeOptical(G4AnalysisManager *analysisManager){

	  //// fill ntuples

  analysisManager->FillNtupleIColumn(1,0, RunNumber);
  analysisManager->FillNtupleIColumn(1,1, fEventID);
  analysisManager->FillNtupleIColumn(1,2, trackID);
  analysisManager->FillNtupleDColumn(1,3, position.x());
  analysisManager->FillNtupleDColumn(1,4, position.y());
  analysisManager->FillNtupleDColumn(1,5, position.z());
  analysisManager->FillNtupleIColumn(1,6, sipmNumber);
  analysisManager->FillNtupleDColumn(1,7, photonEnergy);
  //analysisManager->FillNtupleIColumn(1,7, fDetection);
  analysisManager->AddNtupleRow(1);



	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
