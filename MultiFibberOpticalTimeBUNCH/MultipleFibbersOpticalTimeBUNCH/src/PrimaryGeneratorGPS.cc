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
/// \file eventgenerator/Gun/src/PrimaryGeneratorGPS.cc
/// \brief Implementation of the PrimaryGeneratorfAction class
//
//
// $Id: PrimaryGeneratorGPS.cc 68734 2013-04-05 09:47:02Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorGPS.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4Electron.hh"
#include "Analysis.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSPosDistribution.hh"
#include "DetectorConstruction.hh"
#include "Randomize.hh"
#include "G4VPhysicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorGPS::PrimaryGeneratorGPS(DetectorConstruction* detectorConstruction, G4int fAcquisitionTime,G4int RunNumber)
 : G4VUserPrimaryGeneratorAction(), fParticleGun(0),fParticleEnergy(0),fDetConstruction(detectorConstruction),   fSourcePosX(0),
  fSourcePosY(0),
  fSourcePosZ(0)
 {
  fParticleGun  = new G4GeneralParticleSource();
  
  fParticleGun-> SetVerbosity(0);
  fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());
  fParticleGun->SetNumberOfParticles(1);
  fParticleGun->SetParticleTime(0);
  acquisitionTime=fAcquisitionTime;
	runNumber=RunNumber;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorGPS::~PrimaryGeneratorGPS()
{
  delete fParticleGun;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorGPS::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  //
  
  
   G4SPSPosDistribution* posDist = fParticleGun->GetCurrentSource()->GetPosDist();
   posDist->SetPosDisType("Volume");
   posDist->SetPosDisShape("Cylinder");
   
   G4VPhysicalVolume* randFibberPV;
   G4double sourceRadius=fDetConstruction->GetSourceRadius();
   posDist->SetRadius(sourceRadius);
   
   G4double sourceHalfLength=fDetConstruction->GetSourceHalfLength();
   posDist->SetHalfZ(sourceHalfLength);
   
   
const G4int nFibbers=fDetConstruction->GetNFibbers();
  

   G4int fibberNumber=std::abs(G4UniformRand()*(nFibbers));
   
   G4cout<<"fibberNumber "<<fibberNumber<<G4endl;
   
   
   randFibberPV=fDetConstruction->GetFibbersPV(fibberNumber);
   
	G4ThreeVector sourcePosition=randFibberPV->GetTranslation();
   	
   	  
   posDist->SetCentreCoords(sourcePosition);
   G4String confinedToSolid="sourcePhysical_"+std::to_string(fibberNumber);
    posDist->ConfineSourceToVolume(confinedToSolid); 



if (acquisitionTime>0){//IF ACQUISITIONTIME==0 WE ARE IN EVENTMODE ACQUISITION

	aTime=GenerateTime(acquisitionTime,runNumber);

    fParticleGun->SetParticleTime(aTime);
}

  
  fParticleGun->GeneratePrimaryVertex(anEvent);
   
 fPrimaryVertex= anEvent->GetPrimaryVertex();
  
 fSourcePosX=fPrimaryVertex->GetX0();
 fSourcePosY=fPrimaryVertex->GetY0();
 fSourcePosZ=fPrimaryVertex->GetZ0();
 fSourceT0=fPrimaryVertex->GetT0();
 
 G4cout<<"T= "<<fSourceT0<<G4endl;
 
  ////////////////Distance from vertex to fibber surface
  
  //Source Projection. We do not want z coordinate
  G4ThreeVector vertexPositionRadial=G4ThreeVector(fSourcePosX,fSourcePosY,0.);
  G4ThreeVector fibberPositionRadial=G4ThreeVector(sourcePosition.getX(),sourcePosition.getY(),0.);
  //Distance vector:
	G4ThreeVector distance=vertexPositionRadial-fibberPositionRadial;
//Get the radius in cilindrical coordinates
	G4double radialDistance=distance.getR();
 
 G4double fibberRadius= fDetConstruction->GetFibberRadius();

G4double distanceToSurface=radialDistance-fibberRadius;
 ////////////////// End Distance from vertex to fibber surface
  
  //G4cout<<"vertex "<<vertexPositionRadial<<" fibber " <<fibberPositionRadial<< " distance "<< distance <<" R "<<radialDistance<< " distanceSurface "<<distanceToSurface<<G4endl;
  
  
  
  
  
  
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  fParticleEnergy=fParticleGun->GetParticleEnergy();
  analysisManager->FillNtupleDColumn(0,2, fParticleEnergy);
  analysisManager->FillNtupleDColumn(0,3, fSourcePosX);
  analysisManager->FillNtupleDColumn(0,4, fSourcePosY);
  analysisManager->FillNtupleDColumn(0,5, fSourcePosZ);
  analysisManager->FillNtupleDColumn(0,6, fSourceT0);
  analysisManager->FillNtupleIColumn(0,7, fibberNumber);
  analysisManager->FillNtupleDColumn(0,8, distanceToSurface);
  
}

G4double PrimaryGeneratorGPS::GenerateTime(G4int time,G4int runNumber){
	
	G4double randomTime=G4UniformRand()*time+runNumber*time;//generates a random time between 0 and the acquisitionTime. Tnen sums up the previous time that is RunNumber*acquisitiontime
	return randomTime;
	
	}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
