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
// $Id: fibberSD.cc 87359 2014-12-01 16:04:27Z gcosmo $

//FROM EXAMPLE B2a
//
/// \file fibberSD.cc
/// \brief Implementation of the fibberSD class

#include "fibberSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

fibberSD::fibberSD(const G4String& name, const G4String& hitsCollectionName) 
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
    collectionName.insert("fibberHitsCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

fibberSD::~fibberSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void fibberSD::Initialize(G4HCofThisEvent* hce)
{
    // Create hits collection
    fHitsCollection = new fibberHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hce
    G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool fibberSD::ProcessHits(G4Step* aStep, 
                                     G4TouchableHistory*)
{
    // energy deposit
    G4double edep = aStep->GetTotalEnergyDeposit();

    if (edep==0.) return false;

    fibberHit* newHit = new fibberHit();

    newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    newHit->SetParticlePDG (aStep->GetTrack()->GetDefinition()->GetPDGEncoding());
    //newHit->SetParticleEnergy (aStep->GetTrack()->GetDynamicParticle()->GetKineticEnergy ());
  
    newHit->SetParticleEnergy (aStep->GetPreStepPoint()->GetKineticEnergy ());//Kinetic energy is the energy after the interaction
  
    newHit->SetFibberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
                                               ->GetCopyNumber());
    newHit->SetEdep(edep);
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

    //G4cout<<aStep->GetPreStepPoint()->GetPosition()<<G4endl;


    fHitsCollection->insert( newHit );

    //newHit->Print();

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void fibberSD::EndOfEvent(G4HCofThisEvent*)
{
    //if ( verboseLevel>1 ) {
    G4int nofHits = fHitsCollection->entries();
   

    if (nofHits>0){
        G4cout << G4endl
        << "-------->Hits Collection: in this event they are " << nofHits
        << " hits in the fibbers: " << G4endl;

        (*fHitsCollection)[0]->Print();
        //(*fHitsCollection)[0]->Draw();
    }

    //for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
    //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
