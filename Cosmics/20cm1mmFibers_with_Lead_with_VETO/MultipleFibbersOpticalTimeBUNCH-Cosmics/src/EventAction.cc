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
//#include "PrimaryGeneratorGPS.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "StackingAction.hh"
#include "PMTSD.hh"
#include "PMTHit.hh"
#include "fibberSD.hh"
#include "fibberHit.hh"
#include "vetoSD.hh"
#include "vetoHit.hh"
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
   fCoincidenceFlagFibbers(-1),
   fCoincidenceFlagVetoHigh(-1),
   fCoincidenceFlagVetoDown(-1),
   fCoincidenceFlagVetos(-1),
   fCoincidenceFlag(-1),
   fSecondaryParticlePDG(0),
   fSecondaryParticleEnergy(0)
{
    RunNumber=runNumber;
    hitsCollID = -1;
    hitsCollIDfibbers=-1;
    hitsCollIDveto=-1;
}

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
    fCoincidenceFlagFibbers = -1,
    fCoincidenceFlagVetoHigh = -1,
    fCoincidenceFlagVetoDown = -1,
    fCoincidenceFlagVetos = -1,
    fCoincidenceFlag = -1,
    fSecondaryParticlePDG=0;
    fSecondaryParticleEnergy=0.;

    G4SDManager * SDman = G4SDManager::GetSDMpointer();

    if(hitsCollID<0){
        G4String colNam;
        hitsCollID = SDman->GetCollectionID(colNam="hitsCollectionPMTs");
	hitsCollIDfibbers= SDman->GetCollectionID(colNam="fibberHitsCollection");
        hitsCollIDveto= SDman->GetCollectionID(colNam="vetoHitsCollection");
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
    //G4cout<<"EVENTID "<<fEventID<<G4endl;

    if(hitsCollID<0) return;

    G4HCofThisEvent * HCE = event->GetHCofThisEvent();

    PMTHitsCollection* HC = 0;
    fibberHitsCollection* HCFibber=0;
    vetoHitsCollection* HCVeto=0;
    nHitPMT0=0;
    nHitPMT1=0;
    nHitPMT2=0;
    nHitPMT3=0;
    nHitPMT4=0;
    nHitPMT5=0;
    //nDetectedPMT0=0;
    //nDetectedPMT1=0;

    if(HCE)
    {
        HC = (PMTHitsCollection*)(HCE->GetHC(hitsCollID));
        HCFibber=(fibberHitsCollection*)(HCE->GetHC(hitsCollIDfibbers));
        HCVeto=(vetoHitsCollection*)(HCE->GetHC(hitsCollIDveto));
    }

    if ( HC ) {
        int n_hit = HC->entries();

        for ( int i = 0 ; i < n_hit; i++){
            pmtNumber  = (*HC)[i]->GetPMTNb();

            switch (pmtNumber){
                case 0: nHitPMT0=nHitPMT0+1;
                                        break;
		  
                case 1: nHitPMT1=nHitPMT1+1;
					break;

                case 2: nHitPMT2=nHitPMT2+1;
                                        break;

                case 3: nHitPMT3=nHitPMT3+1;
                                        break;

                case 4: nHitPMT4=nHitPMT4+1;
                                        break;

                case 5: nHitPMT5=nHitPMT5+1;
                                        break;

            }
      
            position = (*HC)[i]->GetPos();
      
            trackID=(*HC)[i]->GetTrackID();
            photonEnergy=(*HC)[i]->GetEnergy();

            //fDetection=(*HC)[i]->GetDetection();

            if(pmtNumber==0 || pmtNumber==1){
                FillTreeOpticalFibber(analysisManager);
            }
            if(pmtNumber==2 || pmtNumber==3 || pmtNumber==4 || pmtNumber==5){
                FillTreeOpticalVeto(analysisManager);
            }
        }
    }

    if(nHitPMT0 > 0 && nHitPMT1 >0){
        fCoincidenceFlagFibbers=1;
    }
    else{
        fCoincidenceFlagFibbers=0;
    }

    if(nHitPMT2 > 0 && nHitPMT3 >0){
        fCoincidenceFlagVetoHigh=1;
    }
    else{
        fCoincidenceFlagVetoHigh=0;
    }

    if(nHitPMT4 > 0 && nHitPMT5 >0){
        fCoincidenceFlagVetoDown=1;
    }
    else{
        fCoincidenceFlagVetoDown=0;
    }

    if(fCoincidenceFlagVetoHigh == 1 && fCoincidenceFlagVetoDown == 1){
        fCoincidenceFlagVetos=1;
    }
    else{
        fCoincidenceFlagVetos=0;
    }

    if(fCoincidenceFlagFibbers == 1 && fCoincidenceFlagVetos == 1){
        fCoincidenceFlag=1;
    }
    else{
        fCoincidenceFlag=0;
    }

    if(HCFibber){
        if(HCFibber->entries()>0){
            fSecondaryParticlePDG  = (*HCFibber)[0]->GetParticlePDG();
            fSecondaryParticleEnergy=(*HCFibber)[0]->GetParticleEnergy();
            //ADD ENERGY
        }

        FillTreeTritiumFibber(analysisManager);
    }

    if(HCVeto){
        if(HCVeto->entries()>0){
            fSecondaryParticlePDG  = (*HCVeto)[0]->GetParticlePDG();
            fSecondaryParticleEnergy=(*HCVeto)[0]->GetParticleEnergy();
            //ADD ENERGY
        }

        FillTreeTritiumVeto(analysisManager);
    }
 

}

//G4double 	GetT0 () const
 
  
void EventAction::FillTreeTritiumFibber(G4AnalysisManager *analysisManager){
    //// fill ntuples
    analysisManager->FillNtupleIColumn(0,0, RunNumber);
    analysisManager->FillNtupleIColumn(0,1, fEventID);
    //analysisManager->FillNtupleDColumn(0,2, fParticleEnergy);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleDColumn(0,3, fSourcePosX);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleDColumn(0,4, fSourcePosY);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleDColumn(0,5, fSourcePosZ);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleDColumn(0,6, fSourceT0);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleIColumn(0,7, fSourceNumber); ->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleIColumn(0,8, distanceToFibber); ->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleIColumn(0,9, fPhotonCounter);->Filled in StackingAction.cc
    analysisManager->FillNtupleDColumn(0,10, fEnergyWater);
    analysisManager->FillNtupleDColumn(0,11, fEnergyFibber);
    analysisManager->FillNtupleDColumn(0,12, fTrackLWater);
    analysisManager->FillNtupleDColumn(0,13, fTrackLFibber);
    analysisManager->FillNtupleIColumn(0,14, nHitPMT0);
    analysisManager->FillNtupleIColumn(0,15, nHitPMT1);
    analysisManager->FillNtupleIColumn(0,16, nHitPMT0+nHitPMT1);
    analysisManager->FillNtupleIColumn(0,17, fCoincidenceFlagFibbers);
    //analysisManager->FillNtupleIColumn(0,17, nDetectedPMT0);
    //analysisManager->FillNtupleIColumn(0,18, nDetectedPMT1);
    //analysisManager->FillNtupleIColumn(0,19, nDetectedPMT0+nDetectedPMT1);
    analysisManager->FillNtupleIColumn(0,22, fSecondaryParticlePDG);
    analysisManager->FillNtupleDColumn(0,23, fSecondaryParticleEnergy);
    analysisManager->FillNtupleIColumn(0,24, fCoincidenceFlag);
    analysisManager->AddNtupleRow(0);
}
  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::FillTreeOpticalFibber(G4AnalysisManager *analysisManager){
    //// fill ntuples

    analysisManager->FillNtupleIColumn(1,0, RunNumber);
    analysisManager->FillNtupleIColumn(1,1, fEventID);
    analysisManager->FillNtupleIColumn(1,2, trackID);
    analysisManager->FillNtupleDColumn(1,3, position.x());
    analysisManager->FillNtupleDColumn(1,4, position.y());
    analysisManager->FillNtupleDColumn(1,5, position.z());
    analysisManager->FillNtupleIColumn(1,6, pmtNumber);
    analysisManager->FillNtupleDColumn(1,7, photonEnergy);
    //analysisManager->FillNtupleIColumn(1,7, fDetection);
    analysisManager->AddNtupleRow(1);
}

void EventAction::FillTreeTritiumVeto(G4AnalysisManager *analysisManager){
    //// fill ntuples
    analysisManager->FillNtupleIColumn(3,0, RunNumber);
    analysisManager->FillNtupleIColumn(3,1, fEventID);
    //analysisManager->FillNtupleDColumn(3,2, fParticleEnergy);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleDColumn(3,3, fSourcePosX);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleDColumn(3,4, fSourcePosY);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleDColumn(3,5, fSourcePosZ);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleDColumn(3,6, fSourceT0);->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleIColumn(3,7, fSourceNumber); ->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleIColumn(3,8, distanceToFibber); ->Filled in PrimaryGeneratorGPS.cc
    //analysisManager->FillNtupleIColumn(3,9, fPhotonCounter);->Filled in StackingAction.cc
    analysisManager->FillNtupleDColumn(3,10, fEnergyWater);
    analysisManager->FillNtupleDColumn(3,11, fEnergyVeto);
    analysisManager->FillNtupleDColumn(3,12, fTrackLWater);
    analysisManager->FillNtupleDColumn(3,13, fTrackLVeto);
    analysisManager->FillNtupleIColumn(3,14, nHitPMT2);
    analysisManager->FillNtupleIColumn(3,15, nHitPMT3);
    analysisManager->FillNtupleIColumn(3,16, nHitPMT2+nHitPMT3);
    analysisManager->FillNtupleIColumn(3,17, fCoincidenceFlagVetoHigh);
    analysisManager->FillNtupleIColumn(3,18, nHitPMT4);
    analysisManager->FillNtupleIColumn(3,19, nHitPMT5);
    analysisManager->FillNtupleIColumn(3,20, nHitPMT4+nHitPMT5);
    analysisManager->FillNtupleIColumn(3,21, fCoincidenceFlagVetoDown);
    analysisManager->FillNtupleIColumn(3,22, nHitPMT2+nHitPMT3+nHitPMT4+nHitPMT5);
    analysisManager->FillNtupleIColumn(3,23, fCoincidenceFlagVetos);
    analysisManager->FillNtupleIColumn(3,24, fCoincidenceFlag);
    //analysisManager->FillNtupleIColumn(3,17, nDetectedPMT0);
    //analysisManager->FillNtupleIColumn(3,18, nDetectedPMT1);
    //analysisManager->FillNtupleIColumn(3,19, nDetectedPMT0+nDetectedPMT1);
    analysisManager->FillNtupleIColumn(3,29, fSecondaryParticlePDG);
    analysisManager->FillNtupleDColumn(3,30, fSecondaryParticleEnergy);
    analysisManager->AddNtupleRow(3);
}
	
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::FillTreeOpticalVeto(G4AnalysisManager *analysisManager){
    //// fill ntuples

    analysisManager->FillNtupleIColumn(4,0, RunNumber);
    analysisManager->FillNtupleIColumn(4,1, fEventID);
    analysisManager->FillNtupleIColumn(4,2, trackID);
    analysisManager->FillNtupleDColumn(4,3, position.x());
    analysisManager->FillNtupleDColumn(4,4, position.y());
    analysisManager->FillNtupleDColumn(4,5, position.z());
    analysisManager->FillNtupleIColumn(4,6, pmtNumber);
    analysisManager->FillNtupleDColumn(4,7, photonEnergy);
    //analysisManager->FillNtupleIColumn(4,7, fDetection);
    analysisManager->AddNtupleRow(4);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
