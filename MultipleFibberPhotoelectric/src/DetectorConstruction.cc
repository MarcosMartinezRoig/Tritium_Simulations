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
// $Id: DetectorConstruction.cc 77656 2013-11-27 08:52:57Z gcosmo $
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "fibberSD.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4RotationMatrix.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"


//static const G4double Degree = 2.*PI;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fVisAttributes()
{}
  //  fMessenger(0),
//  fArmAngle(0.*deg), fArmRotation(0), fSecondArmPhys(0)

//{
//    fArmRotation = new G4RotationMatrix();
//    fArmRotation->rotateY(fArmAngle);
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
//    delete fArmRotation;
//    delete fMessenger;
    
    for (G4int i=0; i<G4int(fVisAttributes.size()); ++i) 
    {
      delete fVisAttributes[i];
    }  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Construct materials
    ConstructMaterials();
    G4Material* water = G4Material::GetMaterial("G4_WATER");
    G4Material* teflon = G4Material::GetMaterial("G4_TEFLON");
    G4Material* polystyrene = G4Material::GetMaterial("G4_POLYSTYRENE");
   G4Material*  PMMA = G4Material::GetMaterial("PMMA");

    G4bool checkOverlaps = true;

    
    // geometries --------------------------------------------------------------
    // experimental hall (world volume)
    G4VSolid* worldSolid 
      = new G4Box("worldBox",10.*cm,10.*cm,50.*cm);
    G4LogicalVolume* worldLogical  = new G4LogicalVolume(worldSolid,water,"worldLogical");
    G4VPhysicalVolume* worldPhysical
      = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,
                          false,0,checkOverlaps);
    

    // Teflon Tube
    G4double halfTeflonLength=40*cm;
    G4double rIntTeflon=25.*mm;
    G4double rExtTeflon=30.*mm;
    G4VSolid* teflonTubeSolid = new G4Tubs("teflonTube",rIntTeflon,rExtTeflon,halfTeflonLength,0.,twopi);
    G4LogicalVolume* teflonTubeLogical = new G4LogicalVolume(teflonTubeSolid,teflon,"teflonTubeLogical");
    G4VPhysicalVolume* teflonTubePhysical = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.*mm),teflonTubeLogical,
                      "teflonTubePhysical",worldLogical,
                      false,0,checkOverlaps);
                      
                      
    	//Make EndCaps
	
	G4double halfLEndCap=10.*mm;
	G4VSolid* endCapSolid=new G4Tubs("fEndCap",0*mm,rIntTeflon,halfLEndCap,0.,twopi);
	
		
 
 	// place fibbers and source within 3 rings 
  
                   
  G4double rInt=0.5*mm;
  rSource=rInt+0.05*mm;
  halfLength=halfTeflonLength-2*halfLEndCap;
  G4double ring_R1=6*mm;
  G4int nFibbersInside=10;
  G4double dPhi1 = twopi/nFibbersInside;
  G4double ring_R2=12*mm;
  G4int nFibbersMiddle=20;
  G4double dPhi2 = twopi/nFibbersMiddle;
  G4double ring_R3=18*mm;
  G4int nFibbersOutside=30;
  G4double dPhi3 = twopi/nFibbersOutside; 
  
  if((nFibbersInside+nFibbersMiddle+nFibbersOutside)!=nTotalFibbers){
	  G4cout<<" "<<G4endl;
	  G4cout<<"The number of fibbers in rings are not equal no the total number of fibbers. Please fix it"<<G4endl;
	  G4cout<<" "<<G4endl;
	  exit(0);
	  }
  
  G4VSolid* sourceVolume = new G4Tubs("sourceVolume",rInt,rSource,halfLength,0.,twopi);
  G4LogicalVolume* sourceLogical = new G4LogicalVolume(sourceVolume,water,"sourceLogical");

  G4int iFibber;
  

  G4RotationMatrix rotm  = G4RotationMatrix();
  rotm.rotateX(0*deg); 
  rotm.rotateY(0*deg);
  rotm.rotateZ(0*deg);
  G4Transform3D transform ;
  G4Transform3D transformEndCap ; 

   //G4SubtractionSolid *sub;
   G4SubtractionSolid *endCapSub;
   
     //Fibbers 
    G4VSolid* fibberSolid=new G4Tubs("fibberVolume",0*mm,rInt,halfTeflonLength,0.,twopi);
   	G4LogicalVolume* fibbersLogical = new G4LogicalVolume(fibberSolid,polystyrene,"fibbersLogical");
   
	G4double phi, dPhiDummy;
	G4double ring_R;
	
  for (iFibber = 0; iFibber < nTotalFibbers ; iFibber++) {
	  
	  
	  if (iFibber<nFibbersInside){
		  dPhiDummy=dPhi1;
		  ring_R=ring_R1;
		  G4cout<<"Centro "<<iFibber<<G4endl;
		  }
	  if (iFibber>=nFibbersInside && iFibber<nFibbersMiddle+nFibbersInside){
		  dPhiDummy=dPhi2;
		  ring_R=ring_R2;
		  G4cout<<"Meio "<<iFibber<<G4endl;
		  }
	  if (iFibber>=nFibbersMiddle+nFibbersInside){
		  dPhiDummy=dPhi3;
		  ring_R=ring_R3;
		  G4cout<<"Fora "<<iFibber<<G4endl;
		  }
	  
    phi = iFibber*dPhiDummy;
    
    G4ThreeVector uz = G4ThreeVector(std::cos(phi),  std::sin(phi),0);     
    G4ThreeVector positionDummy = (ring_R)*uz;
    G4ThreeVector position = G4ThreeVector(positionDummy.getX(),positionDummy.getY(),-halfLength);
    G4ThreeVector positionEndCap = G4ThreeVector(positionDummy.getX(),positionDummy.getY(),0);
    transform = G4Transform3D(rotm,position);
   transformEndCap = G4Transform3D(rotm,positionEndCap);
	
	
	
	if (iFibber==0){
    endCapSub=new G4SubtractionSolid("sub",endCapSolid,fibberSolid,transformEndCap);
    }
    else{
		endCapSub=new G4SubtractionSolid("sub",endCapSub,fibberSolid,transformEndCap);
		}
		
	
	
	sourcePhysical[iFibber]=new G4PVPlacement(transformEndCap,             //rotation,position
                      sourceLogical,            //its logical volume
                      "sourcePhysical_"+itoa(iFibber),             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      iFibber,                 //copy number
                      checkOverlaps);       // che
		
		
		
	
	fibbersPhysical[iFibber]=new G4PVPlacement(transformEndCap,             //rotation,position
                      fibbersLogical,            //its logical volume
                      "fibbersPhysical_"+itoa(iFibber),             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      iFibber,                 //copy number
                      checkOverlaps);       // checking overlaps   
                                   

                     
                      
  }



                    
   ////Place EndCaps in world      
   G4LogicalVolume* endCapLogical = new G4LogicalVolume(endCapSub,PMMA,"endCapLogical");          
  G4VPhysicalVolume* endCapPhysical1=	new G4PVPlacement(0,             //rotation
					G4ThreeVector(0.,0.,halfTeflonLength-halfLEndCap),					//position
                      endCapLogical,            //its logical volume
                      "endCapPhysical",             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      0,                 //copy number
                      checkOverlaps);       // checking overlaps                  
                 
 	  G4VPhysicalVolume* endCapPhysical2=	new G4PVPlacement(0,             //rotation
					G4ThreeVector(0.,0.,-halfTeflonLength+halfLEndCap),					//position
                      endCapLogical,            //its logical volume
                      "endCapPhysical2",             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      1,                 //copy number
                      checkOverlaps);       // checking overlaps    
 	
 	
 	//G4cout<<"NDAUGHTERS "<<worldLogical->GetNoDaughters()<<G4endl;
 	
 	
 	
//    // visualization attributes ------------------------------------------------
    
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    visAttributes->SetVisibility(false);
    worldLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    visAttributes = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
    visAttributes->SetVisibility(true);
    teflonTubeLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    

    visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    visAttributes->SetVisibility(true);
    fibbersLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    visAttributes->SetVisibility(true);
    sourceLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);


 

    // return the world physical volume ----------------------------------------
    
    return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void DetectorConstruction::ConstructSDandField()
//{
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructMaterials()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    // Air 
    nistManager->FindOrBuildMaterial("G4_TEFLON");
  
    // Water
    nistManager->FindOrBuildMaterial("G4_WATER");

    //Polystyrene
    nistManager->FindOrBuildMaterial("G4_POLYSTYRENE");
    
    //PMMA
    //--------------------------------------------------
     // WLSfiber PMMA
     //--------------------------------------------------
    G4double density;
    std::vector<G4int> natoms;
	std::vector<G4String> elements;
   
     elements.push_back("C");     natoms.push_back(5);
     elements.push_back("H");     natoms.push_back(8);
     elements.push_back("O");     natoms.push_back(2);
   
     density = 1.190*g/cm3;
   
  nistManager->ConstructNewMaterial("PMMA", elements, natoms, density);
    
    
    
    
    nistManager->FindOrBuildMaterial("G4_PMMA");

    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
void DetectorConstruction::ConstructSDandField()
{
  //// Sensitive detectors

  G4String fibberSDname = "/fibberSD";
  fibberSD* aFibberSD = new fibberSD(fibberSDname,"fibberHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aFibberSD);
  // Setting aFibberSD to all logical volumes with the same name 
  // of "fibberLogical".
  SetSensitiveDetector("sourceLogical", aFibberSD, true);

}

G4VPhysicalVolume* DetectorConstruction::GetWaterPV(G4int number){
	
	return sourcePhysical[number];
	}


G4VPhysicalVolume* DetectorConstruction::GetFibbersPV(G4int number){
	
	return fibbersPhysical[number];
	}
	
	
G4int DetectorConstruction::GetNFibbers()
{
  return nTotalFibbers;
}	


G4double DetectorConstruction::GetSourceRadius()
{
  return rSource;
}	

G4double DetectorConstruction::GetSourceLength()
{
  return halfLength;
}	


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
