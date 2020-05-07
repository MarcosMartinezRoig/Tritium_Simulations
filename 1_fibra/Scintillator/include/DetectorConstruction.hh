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
/// \file /include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "sipmSD.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Materials;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    G4VPhysicalVolume* GetWaterPV();
    G4VPhysicalVolume* GetFibbersPV();
    G4double GetSourceRadius();
    G4double GetSourceLength();
    G4VPhysicalVolume* fiberPhysical;
    G4VPhysicalVolume* sourcePhysical;
    G4VPhysicalVolume* SilicePhysical1;
    G4VPhysicalVolume* SilicePhysical2;

    virtual void ConstructSDandField();



  //  G4double r_source;
   // G4double half_length_fiber;


  public:
    virtual G4VPhysicalVolume* Construct();

  private:
    G4double world_half_x;
    G4double world_half_y;
    G4double world_half_z;
    G4double half_length_SiPM;


    G4double r_min;
    G4double r_max;


    Materials* fMaterials;
    G4Material *air;
    G4Material *polystyrene;
    G4Material *water;
    G4Material *silice;


    //std::vector<G4VisAttributes*> fVisAttributes;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*DetectorConstruction_h*/
