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
// $Id: PMTSD.cc 87359 2014-12-01 16:04:27Z gcosmo $

//FROM EXAMPLE B2a
//
/// \file PMTSD.cc
/// \brief Implementation of the PMTSD class

#include "PMTSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PMTSD::PMTSD(const G4String& name, const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert("hitsCollectionPMTs");
  //hcID=-1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PMTSD::~PMTSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PMTSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection = new PMTHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PMTSD::ProcessHits(G4Step* aStep, 
                                     G4TouchableHistory*)
{
	  
if ( aStep->GetTrack()->GetDefinition()== G4OpticalPhoton::OpticalPhotonDefinition()) {
	
	//G4int detFlag=0;
  PMTHit* newHit = new PMTHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetPMTNb(aStep->GetPreStepPoint()->GetTouchableHandle()
                                               ->GetCopyNumber());
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
  newHit->SetEnergy  (aStep->GetTrack()->GetTotalEnergy () );


//G4cout<<"PHOTON ENERG "<<aStep->GetTrack()->GetTotalEnergy ()<<G4endl;
   //if ( aStep->GetTrack()->GetDefinition()== G4OpticalPhoton::OpticalPhotonDefinition()) {
  
  //G4OpBoundaryProcessStatus theStatus = Undefined;

  //G4ProcessManager* OpManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  //if (OpManager) {
     //G4int MAXofPostStepLoops =
              //OpManager->GetPostStepProcessVector()->entries();
     //G4ProcessVector* fPostStepDoItVector = OpManager->GetPostStepProcessVector(typeDoIt);

     //for ( G4int i=0; i<MAXofPostStepLoops; i++) {
         //G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
        //G4OpBoundaryProcess* fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
         //if (fOpProcess) { theStatus = fOpProcess->GetStatus(); break;}
     //}
  //}

//if (theStatus==Detection){
	
	//detFlag=1;
	//}
	
	
	
	//newHit->SetDetection(detFlag);
  

  fHitsCollection->insert( newHit );
  
}
  //newHit->Print();

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PMTSD::EndOfEvent(G4HCofThisEvent*)
{
  //if ( verboseLevel>1 ) { 
     //G4int nofHits = fHitsCollection->entries();
     //G4cout << G4endl
            //<< "-------->Hits Collection: in this event they are " << nofHits 
            //<< " hits in the PMTs: " << G4endl;
     //for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
