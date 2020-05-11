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
// $Id: DetectorConstruction.hh 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "fibberSD.hh"
#include "G4Cache.hh"
#include "PMTSD.hh"
#include <vector>

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4VisAttributes;
class G4GenericMessenger;
class Materials;

/// Detector construction

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
// get methods
    //
    G4VPhysicalVolume* GetWaterPV(G4int);
    G4VPhysicalVolume* GetFibbersPV(G4int);
    G4int GetNFibbers();
    G4double GetSourceRadius();
    G4double GetSourceHalfLength();
     G4double GetFibberLength();
    G4double GetFibberRadius();
    G4Material* FindMaterial(G4String);
    G4double GetDetectionArea();
    void ExportFibbersPositions();

    

	//static const G4int nTotalFibbers = 350;
	G4int nTotalFibbers, iFibber;
    G4double rSource,halfLength,rInt, fibberHalfLength;
	
    G4VPhysicalVolume*   sourcePhysical[5000]; 
    G4VPhysicalVolume*   fibbersPhysical[5000];
	G4LogicalVolume* fibbersLogical;
    
    G4String itoa(int current)
	{
    const char theDigits[11] = "0123456789";
     G4String result;
     int digit;
     do
     {
       digit = current-10*(current/10);
       result=theDigits[digit]+result;
       current/=10;
     }
     while(current!=0);
     return result;
	}

private:

    G4GenericMessenger* fMessenger;
    std::vector<G4VisAttributes*> fVisAttributes;
    Materials* fMaterials;
    G4Material *water;
    G4Material* teflon;
    G4Material* polystyrene;
    G4Material*  PMMA;
    G4Material*  Silicone;
	G4Material* glass;
	G4Material* bialkaliPhoto;
	
	//Sensitive detectors
	//PMT_SD* fPmt_SD;
};




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
