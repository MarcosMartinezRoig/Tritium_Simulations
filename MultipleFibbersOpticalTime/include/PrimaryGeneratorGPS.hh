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
/// \file eventgenerator/Gun/include/PrimaryGeneratorGPS.hh
/// \brief Definition of the PrimaryGeneratorAction class
//
//
// $Id: PrimaryGeneratorGPS.hh 68734 2013-04-05 09:47:02Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorGPS_h
#define PrimaryGeneratorGPS_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4PrimaryVertex.hh"


class G4GeneralParticleSource;
class G4Event;
class DetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorGPS : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorGPS(DetectorConstruction* detectorConstruction, G4int,G4int );    
   ~PrimaryGeneratorGPS();
   
	
  
  public:
    virtual void GeneratePrimaries(G4Event*);
	G4double GenerateTime(G4int,G4int);
	G4int acquisitionTime;
	G4int runNumber;

	
  private:
    G4GeneralParticleSource*  fParticleGun;
	G4double fParticleEnergy;
	DetectorConstruction* fDetConstruction;
	G4PrimaryVertex* fPrimaryVertex;
	G4double  fSourcePosX;
    G4double  fSourcePosY;
    G4double  fSourcePosZ;
     G4double  fSourceT0;
     G4double aTime;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
