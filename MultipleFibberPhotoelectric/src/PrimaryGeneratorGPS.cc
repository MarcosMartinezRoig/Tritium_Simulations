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

PrimaryGeneratorGPS::PrimaryGeneratorGPS(DetectorConstruction* detectorConstruction)
 : G4VUserPrimaryGeneratorAction(), fParticleGun(0),fParticleEnergy(0),fDetConstruction(detectorConstruction)
 {
  fParticleGun  = new G4GeneralParticleSource();
  
  fParticleGun-> SetVerbosity(2);
  fParticleGun->SetParticleDefinition(G4Electron::ElectronDefinition());
//  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.*mm));
  fParticleGun->SetNumberOfParticles(1);
  //fParticleGun-> SetCurrentSourceIntensity (1000.);
  
  
  
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
   
   G4double sourceHalfLength=fDetConstruction->GetSourceLength();
   posDist->SetHalfZ(sourceHalfLength);
   
   
const G4int nFibbers=fDetConstruction->GetNFibbers();
  

   //G4cout<<"NFibbers "<<nFibbers<<G4endl;
   G4int fibberNumber=std::abs(G4UniformRand()*(nFibbers));
   G4cout<<"Rand "<<fibberNumber<<G4endl;
   
   
   randFibberPV=fDetConstruction->GetWaterPV(fibberNumber);
	G4ThreeVector sourcePosition=randFibberPV->GetTranslation();
   	
   	
   	G4cout<<"PV "<<randFibberPV->GetName()<<G4endl;
  
   posDist->SetCentreCoords(sourcePosition);
   G4String confinedToSolid=randFibberPV->GetName();;
   //confinedToSolid="sourcePhysical_"+std::to_string(fibberNumber);
   posDist->ConfineSourceToVolume(confinedToSolid); 

  
  fParticleGun->GeneratePrimaryVertex(anEvent);
  
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  fParticleEnergy=fParticleGun->GetParticleEnergy();
  analysisManager->FillNtupleDColumn(2, fParticleEnergy);
  analysisManager->FillNtupleIColumn(6, fibberNumber);
  
  
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
