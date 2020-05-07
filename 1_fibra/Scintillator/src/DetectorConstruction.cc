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
/// \file /src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "Materials.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "sipmSD.hh"
#include "sipmHit.hh"
#include "G4SDManager.hh"



G4double r_source = 0.55*mm;
G4double half_length_fiber = 10*cm;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(), fMaterials(NULL), air(NULL), polystyrene(NULL), water(NULL), silice(NULL)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  world_half_x= 5*cm;
  world_half_y= 5*cm;
  world_half_z= 14*cm;
  half_length_SiPM= 1*mm;

  r_min = 0*mm;
  r_max = 0.5*mm;




  fMaterials = Materials::GetInstance();
  air=G4Material::GetMaterial("G4_AIR",true);
  polystyrene=G4Material::GetMaterial("G4_POLYSTYRENE",true);
  water=G4Material::GetMaterial("G4_WATER",true);
  silice=G4Material::GetMaterial("G4_Si",true);




  G4bool checkOverlaps = false;

  //world
  G4VSolid* worldSolid = new G4Box("worldBox",world_half_x, world_half_y, world_half_z);
  G4LogicalVolume* worldLogical = new G4LogicalVolume(worldSolid,water,"worldLogical");
  G4VPhysicalVolume* worldPhysical = new G4PVPlacement(0, G4ThreeVector(), worldLogical, "world", 0, false, 0, checkOverlaps);

  //fiber
  G4VSolid* fiberSolid = new G4Tubs("fiber",r_min, r_max, half_length_fiber,0*deg,360*deg);
  G4LogicalVolume* fiberLogical = new G4LogicalVolume(fiberSolid,polystyrene,"fiberLogical");
  fiberPhysical = new G4PVPlacement(0, G4ThreeVector(), fiberLogical, "fiberphysical", worldLogical, false, 0, checkOverlaps);

  //source
  G4VSolid* sourceSolid = new G4Tubs("source",r_max, r_source, half_length_fiber,0*deg,360*deg);
  G4LogicalVolume* sourceLogical = new G4LogicalVolume(sourceSolid,water,"sourceLogical");
  sourcePhysical = new G4PVPlacement(0, G4ThreeVector(), sourceLogical, "sourcephysical", worldLogical, false, 0, checkOverlaps);

  //source
  G4VSolid* SiliceSolid = new G4Tubs("Silice",r_min, r_max, half_length_SiPM,0*deg,360*deg);
  G4LogicalVolume* SiliceLogical = new G4LogicalVolume(SiliceSolid,water,"SiliceLogical");
  SilicePhysical1 = new G4PVPlacement(0, G4ThreeVector(0,0,half_length_fiber+half_length_SiPM), SiliceLogical, "Silicephysical", worldLogical, true, 0, checkOverlaps);
  SilicePhysical2 = new G4PVPlacement(0, G4ThreeVector(0,0,-(half_length_fiber+half_length_SiPM)), SiliceLogical, "Silicephysical", worldLogical, true, 1, checkOverlaps);



  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,0.0));

  visAttributes->SetVisibility(true);https://www.google.es/
  worldLogical->SetVisAttributes(visAttributes);
  //fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  visAttributes->SetVisibility(true);
  fiberLogical->SetVisAttributes(visAttributes);
  //fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  visAttributes->SetVisibility(true);
  sourceLogical->SetVisAttributes(visAttributes);
  //fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(true);
  SiliceLogical->SetVisAttributes(visAttributes);
  //fVisAttributes.push_back(visAttributes);

  // PMMA-Water

G4OpticalSurface* opFiberSurceSurface = new G4OpticalSurface("opfibersurcesurface");


        opFiberSurceSurface->SetModel(unified);                  // SetModel
        opFiberSurceSurface->SetType(dielectric_dielectric);   // SetType
        opFiberSurceSurface->SetFinish(polished);                 // SetFinish

G4OpticalSurface* opWaterSurceSurface = new G4OpticalSurface("opwatersurcesurface");

        opWaterSurceSurface->SetModel(unified);                  // SetModel
        opWaterSurceSurface->SetType(dielectric_dielectric);   // SetType
        opWaterSurceSurface->SetFinish(polished);                 // SetFinish


G4OpticalSurface* opSipm=    new G4OpticalSurface("opSipm",glisur,ground,dielectric_metal);

        G4MaterialPropertiesTable *siMPT=fMaterials->GetPhotoMPT();
        opSipm->SetMaterialPropertiesTable(siMPT);


G4LogicalBorderSurface* FiberSurceSurface = new G4LogicalBorderSurface("FiberSurceSurface1",fiberPhysical,sourcePhysical,opFiberSurceSurface);
G4LogicalBorderSurface* FiberWaterSurface = new G4LogicalBorderSurface("FiberSurceSurface2",fiberPhysical,worldPhysical,opFiberSurceSurface);
G4LogicalBorderSurface* WaterSurceSurface = new G4LogicalBorderSurface("WaterSurceSurface2",sourcePhysical,worldPhysical,opWaterSurceSurface);

  ////**Create logical skin surfaces
G4LogicalSkinSurface *sipmSkinSurf=new G4LogicalSkinSurface("SiPMSurface",SiliceLogical,opSipm);





return worldPhysical;
}


G4VPhysicalVolume* DetectorConstruction::GetWaterPV(){

    return sourcePhysical;
}



G4VPhysicalVolume* DetectorConstruction::GetFibbersPV(){

    return fiberPhysical;
    }





G4double DetectorConstruction::GetSourceRadius()
{

//G4cout<<"TESTE "<<r_source<<G4endl;
  return r_source;
}

G4double DetectorConstruction::GetSourceLength()
{
  return half_length_fiber;
}

void DetectorConstruction::ConstructSDandField()
{

   //SiPM SD

   sipmSD* SipmSD = new sipmSD("SipmSD","HitsCollectionSiPMs");
   G4SDManager::GetSDMpointer()->AddNewDetector(SipmSD);
   SetSensitiveDetector("SiliceLogical", SipmSD,true);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
