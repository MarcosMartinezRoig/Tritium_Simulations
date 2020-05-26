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
// $Id: Materials.hh 69561 2013-05-08 12:25:56Z gcosmo $
//
/// \file optical/wls/include/Materials.hh
/// \brief Definition of the Materials class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef Materials_h
#define Materials_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4MaterialPropertiesTable.hh"

class Materials
{
  public:

    virtual ~Materials();
 
    static Materials* GetInstance();

    G4Material* GetMaterial(const G4String);
	G4MaterialPropertiesTable* GetPhotoMPT();
	 G4MaterialPropertiesTable* mptPhoto;
	
  private:
 
    Materials();

    void CreateMaterials();
    

  private:

    static Materials* fInstance;

    G4NistManager*     fNistMan;
    G4Material*        fWater;
    G4Material*        fAir;
    G4Material*        fPMMA;
    G4Material*        fPolystyrene;
     G4Material*        fTeflon;
    G4Material*        fSilicone;
    G4Material* fGlass;
	G4Material* fPhotocathode;
};

#endif /*Materials_h*/
