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
//#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
//#include "G4MaterialPropertiesTable.hh"
#include "Materials.hh"
//Optical surfaces
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"


//static const G4double Degree = 2.*PI;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), 
  fVisAttributes(),
  fMaterials(NULL)
{}
 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    for (G4int i=0; i<G4int(fVisAttributes.size()); ++i)
    {
        delete fVisAttributes[i];
    }

    if (fMaterials)  delete fMaterials;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Construct materials
  
    fMaterials = Materials::GetInstance();

    air=FindMaterial("G4_AIR");
    lead=FindMaterial("G4_Pb");
    water=FindMaterial("G4_WATER");
    teflon = FindMaterial("G4_TEFLON");
    polystyrene= FindMaterial("G4_POLYSTYRENE");
    PMMA= FindMaterial("PMMA");
    Silicone= FindMaterial("Silicone");
    glass= FindMaterial("glass");
    bialkaliPhoto= FindMaterial("photocathode");

    G4bool checkOverlaps = true;

    
    // geometries --------------------------------------------------------------
  
    

    G4double worldExt=150*cm;//use 150cm as the Cry plan was choosed to have 3m side
    G4double worldHeight=40*cm;

    //External lead size
    G4double halflengthExtLeadX = 74*cm;
    G4double halflengthExtLeadY = 30*cm;
    G4double halflengthExtLeadZ = 35*cm;
    G4double thicknessLead=5*cm;

    //Internal lead size
    G4double halflengthIntLeadX = halflengthExtLeadX-thicknessLead;
    G4double halflengthIntLeadY = halflengthExtLeadY-thicknessLead;
    G4double halflengthIntLeadZ = halflengthExtLeadZ-thicknessLead;

    G4double halfLEndCap=10.*mm;
    fibberHalfLength=10.0*cm;//0 mm from the endcap
    G4double halfTeflonLength=fibberHalfLength+2*halfLEndCap;
    G4double rIntTeflon=18.0*mm;
    G4double rExtTeflon=30.0*mm;

    fibberHalfLength=halfTeflonLength-2*halfLEndCap;//0 mm from the endcap
    halfLength=fibberHalfLength;
    G4double halfLengthPMT=10.*mm;
    G4double zTranslaction=-halfTeflonLength;
    //G4double worldHalfLength=3*halfTeflonLength;
    G4double halfLengthSilicone=0.25*mm;
    G4double halfLengthPhotocathode=0.5*mm;
    G4double halfLengthAirTubeDummy=halfTeflonLength+2*halfLengthSilicone+2*halfLengthPMT;
    
    G4RotationMatrix rotm  = G4RotationMatrix();
    rotm.rotateX(0*deg); 
    rotm.rotateY(0*deg);
    rotm.rotateZ(0*deg);
    G4Transform3D transform ;
    G4Transform3D transformEndCap;
    G4Transform3D transformLead;
    G4double phi;
    //G4double dPhiDummy;
    //G4double ring_R;
    G4ThreeVector uz;     
    G4ThreeVector positionDummy;
    G4ThreeVector positionLead;
    G4ThreeVector position;
    G4ThreeVector positionEndCap;
    G4ThreeVector positionDummyAir;

    //world volume
    G4VSolid* worldSolid = new G4Box("worldBox",worldExt,worldExt,worldHeight);
    G4LogicalVolume* worldLogical  = new G4LogicalVolume(worldSolid,water,"worldLogical");
    G4VPhysicalVolume* worldPhysical= new G4PVPlacement(0,G4ThreeVector(0,0,0),worldLogical,
                                                        "worldPhysical",0,false,0,checkOverlaps);
    
    //Make Dummy Tube (in order that the world can be defined as water)
    G4VSolid* airTubeSolidDummy = new G4Tubs("airTubeSolidDummy",0,rExtTeflon,halfLengthAirTubeDummy,0.,twopi);
   
    //Make Air Box
    G4VSolid* airBoxSolid = new G4Box("airBoxSolid",worldExt,worldExt,worldHeight);
    positionDummyAir=G4ThreeVector(0,0,zTranslaction);
    transform = G4Transform3D(rotm,positionDummyAir);
    G4SubtractionSolid *airSolid=new G4SubtractionSolid("airSolid",airBoxSolid,airTubeSolidDummy,transform);
    G4LogicalVolume* airBoxLogical = new G4LogicalVolume(airSolid,air,"airBoxLogical");
    G4VPhysicalVolume* airBoxPhysical = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),airBoxLogical,
                      "airBoxPhysical",worldLogical,false,0,checkOverlaps);

    //Make Lead shielding
    //External box
    G4VSolid* leadBoxExtSolid = new G4Box("leadBoxSolid", halflengthExtLeadX, halflengthExtLeadY, halflengthExtLeadZ);
    //Internal box
    G4VSolid* leadBoxIntSolid = new G4Box("leadBoxSolid", halflengthIntLeadX, halflengthIntLeadY, halflengthIntLeadZ);

    positionLead = G4ThreeVector(0,0,0);
    transformLead = G4Transform3D(rotm,positionLead);

    G4SubtractionSolid* leadBoxSolid = new G4SubtractionSolid("leadSolid",leadBoxExtSolid,leadBoxIntSolid, transformLead);

    G4LogicalVolume* leadBoxLogical = new G4LogicalVolume(leadBoxSolid, lead, "leadBoxLogical");
    G4VPhysicalVolume* leadBoxPhysical = new G4PVPlacement(0, G4ThreeVector(0,0,0), leadBoxLogical, "leadBoxPhysical",
                                                           airBoxLogical, false, 0, checkOverlaps);
    
    //Make Teflon(PTFE) Tube
    G4VSolid* teflonTubeSolid = new G4Tubs("teflonTube",rIntTeflon,rExtTeflon,halfTeflonLength,0.,twopi);
    G4LogicalVolume* teflonTubeLogical = new G4LogicalVolume(teflonTubeSolid,teflon,"teflonTubeLogical");
    G4VPhysicalVolume* teflonTubePhysical = new G4PVPlacement(0,G4ThreeVector(0.,0.,zTranslaction+0.*mm),
                                teflonTubeLogical,"teflonTubePhysical",worldLogical,false,0,checkOverlaps);
                      
    //Make EndCaps
    G4VSolid* endCapSolid=new G4Tubs("fEndCap",0*mm,rIntTeflon,halfLEndCap,0.,twopi);

    //Make Silicone
    G4VSolid* siliconeSolid = new G4Tubs("siliconeSolid",0,rExtTeflon,halfLengthSilicone,0.,twopi);

    //Make PMTs
    G4VSolid* pmtSolid = new G4Tubs("pmtSolid",0,rExtTeflon,halfLengthPMT,0.,twopi);
    //G4VSolid* pmtSolid = new G4Tubs("pmtSolid",0,rIntTeflon,halfLengthPMT,0.,twopi);

    //Make Photocathode
    G4VSolid* photocathodeSolid = new G4Tubs("photocathodeSolid",0,rIntTeflon,halfLengthPhotocathode,0.,twopi);


    //FIBBERS ANS SOURCES -> geometric explanation in blue logbook, page 349
    // place fibbers and source within 3 rings
  
                   
    rInt=0.5*mm;//1mm fibbers
    rSource=rInt+0.005*mm;

    
    const G4int nRings=int(rIntTeflon/(2*rSource));//number of circunferences
    //
    //G4int nRings=22;
    
    G4cout << G4endl;
    G4cout << "rIntTeflon: " << rIntTeflon << G4endl;
    G4cout << "rSource: " << rSource << G4endl;
    G4cout << G4endl << "Number of rings: " << nRings << G4endl;
    
    G4double ringRadius[nRings];
    G4int nFibbers[nRings];
    G4double dPhi[nRings];
    nTotalFibbers=0;
    
    for (int r=0;r<nRings;r++){
        ringRadius[r]=r*2*(rSource);//rSource=rInt+0.05 is the fibber radius

        if (r==0){
            nFibbers[0]=1;//central fibber
        }
        else{
            nFibbers[r]=int(twopi*ringRadius[r]/(2*(rSource)));// blue logbook page 348

            ////AROUCA: change in order to get the same area of the 1m detector: ie, nFibbers(25cm)=nFibbers(1m)*4
            //if (r==nRings-1){nFibbers[r]=54;}
            ///////////////////////////////////////////////////
        }
			
        dPhi[r]=twopi/nFibbers[r];
        G4cout<<"\nRing "<<r<<", radius "<<ringRadius[r]<<" mm, nFibbers "<<nFibbers[r]<<G4endl;
        nTotalFibbers=nTotalFibbers+nFibbers[r];
    }
    
    G4cout<<"\n Total number of fibbers "<<nTotalFibbers<<"\n"<<G4endl;

    //G4VSolid* sourceVolume = new G4Tubs("sourceVolume",rInt,rSource,halfLength,0.,twopi);

    //G4LogicalVolume* sourceLogical = new G4LogicalVolume(sourceVolume,water,"sourceLogical");

    //Fibbers
      
    G4VSolid* fibberSolid=new G4Tubs("fibberVolume",0*mm,rInt,fibberHalfLength,0.,twopi);//fibber not entering the endcap
    G4LogicalVolume* fibbersLogical = new G4LogicalVolume(fibberSolid,polystyrene,"fibbersLogical");

    G4int fibberCounter=0;

    for(int r=0;r<nRings;r++){
        for (iFibber=0;iFibber<nFibbers[r];iFibber++){
            if (r==0){
                positionEndCap = G4ThreeVector(0,0,0+zTranslaction);//central fibber
            }
            else{
                phi = iFibber*dPhi[r];
                uz = G4ThreeVector(std::cos(phi),  std::sin(phi),0);
                positionDummy = (ringRadius[r])*uz;
                position = G4ThreeVector(positionDummy.getX(),positionDummy.getY(),-halfLength+zTranslaction);
                positionEndCap = G4ThreeVector(positionDummy.getX(),positionDummy.getY(),0+zTranslaction);
                transform = G4Transform3D(rotm,position);
            }

            transformEndCap = G4Transform3D(rotm,positionEndCap);

            fibbersPhysical[fibberCounter]=new G4PVPlacement(transformEndCap,             //rotation,position
								  fibbersLogical,            //its logical volume
								  "fibbersPhysical_"+itoa(fibberCounter),             //its name
								  worldLogical,             //its mother  volume
								  true,                 //no boolean operation
								  fibberCounter,                 //copy number
								  checkOverlaps);       // checking overlaps   

            fibberCounter++;
        }
		
    }

    positionEndCap=G4ThreeVector(0,0,-halfLEndCap);
    transformEndCap = G4Transform3D(rotm,positionEndCap);
    //endCapSub=new G4SubtractionSolid("sub",endCapSub,supportRemoveSolid,transformEndCap);

    ////Place EndCaps in world
    G4LogicalVolume* endCapLogical = new G4LogicalVolume(endCapSolid,PMMA,"endCapLogical");
    G4VPhysicalVolume* endCapPhysical1=	new G4PVPlacement(0,             //rotation
					G4ThreeVector(0.,0.,halfTeflonLength-halfLEndCap+zTranslaction),					//position
                      endCapLogical,            //its logical volume
                      "endCapPhysical",             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      0,                 //copy number
                      checkOverlaps);       // checking overlaps                  
 
    G4RotationMatrix *rotEndCap2  = new G4RotationMatrix();
    rotEndCap2->rotateX(180*deg);
    rotEndCap2->rotateY(0*deg);
    rotEndCap2->rotateZ(0*deg);
  
    G4VPhysicalVolume* endCapPhysical2=	new G4PVPlacement(rotEndCap2,             //rotation
					G4ThreeVector(0.,0.,-halfTeflonLength+halfLEndCap+zTranslaction),					//position
                      endCapLogical,            //its logical volume
                      "endCapPhysical2",             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      1,                 //copy number
                      checkOverlaps);       // checking overlaps    
 
 
 
 
    ////Place silicone in world
    G4LogicalVolume* siliconeLogical = new G4LogicalVolume(siliconeSolid,Silicone,"siliconeLogical");
    G4VPhysicalVolume* siliconePhysical1=	new G4PVPlacement(0,             //rotation
					G4ThreeVector(0.,0.,halfTeflonLength+halfLengthSilicone+zTranslaction),					//position
                      siliconeLogical,            //its logical volume
                      "siliconePhysical1",             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      0,                 //copy number
                      checkOverlaps);       // checking overlaps                  
                 
    G4VPhysicalVolume* siliconePhysical2=	new G4PVPlacement(0,             //rotation
					G4ThreeVector(0.,0.,-halfTeflonLength-halfLengthSilicone+zTranslaction),					//position
                      siliconeLogical,            //its logical volume
                      "siliconePhysical2",             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      1,                 //copy number
                      checkOverlaps);       // checking overlaps    
 	
 	
    ////Place PMT in world
 
 
    G4LogicalVolume* pmtLogical = new G4LogicalVolume(pmtSolid,glass,"pmtLogical");
    G4LogicalVolume* photocathodeLogical = new G4LogicalVolume(photocathodeSolid,bialkaliPhoto,"photocathodeLogical");
 
    G4VPhysicalVolume* pmtPhysical = new G4PVPlacement(0,G4ThreeVector(0,0,-halfLengthPMT+2*halfLengthPhotocathode),
                                    photocathodeLogical,
                                    "pmtAndPhotoPhysical",
                                    pmtLogical,
                                    false,
                                    0,
                                    checkOverlaps);
 
 
 
    G4VPhysicalVolume* pmtPhysical1=	new G4PVPlacement(0,             //rotation
					G4ThreeVector(0.,0.,halfTeflonLength+2*halfLengthSilicone+halfLengthPMT+zTranslaction),					//position
                     pmtLogical,            //its logical volume
                      "pmtPhysical1",             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      0,                 //copy number
                      checkOverlaps);       // checking overlaps  
                      
                                      
    G4RotationMatrix rotPMT2  = G4RotationMatrix();
    rotPMT2.rotateX(0*deg);
    rotPMT2.rotateY(180*deg);
    rotPMT2.rotateZ(0*deg);
    G4ThreeVector positionPMT2 =  G4ThreeVector(0.,0.,-halfTeflonLength-2*halfLengthSilicone-halfLengthPMT+zTranslaction);
  
  
    G4Transform3D transformPMT2 = G4Transform3D(rotPMT2,positionPMT2);

    G4VPhysicalVolume* pmtPhysical2=	new G4PVPlacement(transformPMT2,
                      pmtLogical,            //its logical volume
                      "pmtPhysical2",             //its name
                      worldLogical,             //its mother  volume
                      true,                 //no boolean operation
                      1,                 //copy number
                      checkOverlaps);       // checking overlaps    
 		
 	
 	
    //G4cout<<"NDAUGHTERS "<<worldLogical->GetNoDaughters()<<G4endl;
 	
 	
    // ------------- Surfaces --------------
    //
    // Water-Fibbers
    G4int fSurfaceRoughness=0.7;
    G4OpticalSurface* opWaterSurface = new G4OpticalSurface("WaterSurface");          // Surface Name
    opWaterSurface->SetModel(unified);                  // SetModel
    opWaterSurface->SetType(dielectric_dielectric);   // SetType
    opWaterSurface->SetFinish(polished);                 // SetFinish
     
    G4LogicalBorderSurface* WaterFibberSurface[nTotalFibbers];
    G4LogicalBorderSurface* WorldFibberSurface[nTotalFibbers];

    for (iFibber = 0; iFibber < nTotalFibbers ; iFibber++) {
        WaterFibberSurface[iFibber] = new G4LogicalBorderSurface("WaterFibberSurface"+itoa(iFibber),fibbersPhysical[iFibber],sourcePhysical[iFibber],opWaterSurface);
	WorldFibberSurface[iFibber] = new G4LogicalBorderSurface("WorldFibberSurface"+itoa(iFibber),fibbersPhysical[iFibber],worldPhysical,opWaterSurface);
    }

    // Fibbers-PMMA
    G4OpticalSurface* opPS_PMMA = new G4OpticalSurface("PS-PMMASurface",          // Surface Name
                                      glisur,                  // SetModel
                                      ground,                  // SetFinish
                                      dielectric_dielectric,   // SetType
                                      fSurfaceRoughness);      // SetPolish


    G4LogicalBorderSurface* PMMAFibberSurface1[nTotalFibbers];
    G4LogicalBorderSurface* PMMAFibberSurface2[nTotalFibbers];
    for (iFibber = 0; iFibber < nTotalFibbers ; iFibber++) {
    PMMAFibberSurface1[iFibber] = new G4LogicalBorderSurface("PMMAFibberSurface1"+itoa(iFibber),fibbersPhysical[iFibber],endCapPhysical1,opPS_PMMA);
    PMMAFibberSurface2[iFibber] = new G4LogicalBorderSurface("PMMAFibberSurface2"+itoa(iFibber),fibbersPhysical[iFibber],endCapPhysical2,opPS_PMMA);
    }


    // PMMA-Water
    G4OpticalSurface* opPMMA_Water = new G4OpticalSurface("PMMAWaterSurface");

    opPMMA_Water->SetModel(unified);                  // SetModel
    opPMMA_Water->SetType(dielectric_dielectric);   // SetType
    opPMMA_Water->SetFinish(polished);                 // SetFinish
     
    G4LogicalBorderSurface* PMMAWaterSurface1 = new G4LogicalBorderSurface("PMMAWaterSurface1",endCapPhysical1,worldPhysical,opPMMA_Water);
    G4LogicalBorderSurface* PMMAWaterSurface2 = new G4LogicalBorderSurface("PMMAWaterSurface2",endCapPhysical2,worldPhysical,opPMMA_Water);
    //G4LogicalBorderSurface* PMMAWaterSurface3 = new G4LogicalBorderSurface("PMMAWaterSurface3",supportEndCapsPhysical,worldPhysical,opPMMA_Water);

    // PMMA-Silicone
    G4OpticalSurface* opPMMA_Silicone = new G4OpticalSurface("PMMASiliconeSurface");

        opPMMA_Silicone->SetModel(unified);                  // SetModel
        opPMMA_Silicone->SetType(dielectric_dielectric);   // SetType
        opPMMA_Silicone->SetFinish(polished);                 // SetFinish
     
    G4LogicalBorderSurface* PMMASiliconeSurface1 = new G4LogicalBorderSurface("PMMASiliconeSurface1",endCapPhysical1,siliconePhysical1,opPMMA_Silicone);
    G4LogicalBorderSurface* PMMASiliconeSurface2 = new G4LogicalBorderSurface("PMMASiliconeSurface2",endCapPhysical2,siliconePhysical2,opPMMA_Silicone);

    // PMMA-Teflon
    G4int fSurfaceRoughnessTeflon=0.9;

    G4OpticalSurface* opPMMA_PTFE = new G4OpticalSurface("PMMAPTFESurface");

        opPMMA_PTFE->SetModel(glisur);                  // SetModel
        opPMMA_PTFE->SetType(dielectric_metal);   // SetType
        opPMMA_PTFE->SetFinish(ground);                 // SetFinish
        opPMMA_PTFE->SetPolish(fSurfaceRoughnessTeflon);// SetPolish



    G4LogicalBorderSurface* PMMAPTFE1 = new G4LogicalBorderSurface("PMMAPFTE1",endCapPhysical1,teflonTubePhysical,opPMMA_PTFE);
    G4LogicalBorderSurface* PMMAPTFE2 = new G4LogicalBorderSurface("PMMAPTFE2",endCapPhysical2,teflonTubePhysical,opPMMA_PTFE);

    // Teflon-Water

    G4OpticalSurface* opPTFE_Water = new G4OpticalSurface("PTFEWaterSurface");

        opPTFE_Water->SetModel(glisur);                  // SetModel
        opPTFE_Water->SetType(dielectric_metal);   // SetType
        opPTFE_Water->SetFinish(ground);                 // SetFinish
        opPTFE_Water->SetPolish(fSurfaceRoughnessTeflon);// SetPolish

    G4LogicalBorderSurface* PTFEWater = new G4LogicalBorderSurface("PFTEWater",worldPhysical,teflonTubePhysical,opPTFE_Water);

    //SILICON-GLASS
    G4OpticalSurface* opSilicon_Glass = new G4OpticalSurface("SiliconGlassSurface");
        opSilicon_Glass->SetModel(unified);                  // SetModel
        opSilicon_Glass->SetType(dielectric_dielectric);   // SetType
        opSilicon_Glass->SetFinish(polished);                 // SetFinish
     
    G4LogicalBorderSurface* SiliconGlass1 = new G4LogicalBorderSurface("SiliconGlass1",siliconePhysical1,pmtPhysical1,opSilicon_Glass);
    G4LogicalBorderSurface* SiliconGlass2 = new G4LogicalBorderSurface("SiliconGlass2",siliconePhysical2,pmtPhysical2,opSilicon_Glass);

    //GLASS-PHOTOCATHODE
    G4OpticalSurface* opPhotocathode=    new G4OpticalSurface("opPhotocathode",glisur,ground,dielectric_metal);

    G4MaterialPropertiesTable *photoMPT=fMaterials->GetPhotoMPT();
        opPhotocathode->SetMaterialPropertiesTable(photoMPT);

    ////**Create logical skin surfaces
    G4LogicalSkinSurface *photoSkinSurf=new G4LogicalSkinSurface("PhotocathodeSurface",photocathodeLogical,opPhotocathode);

    // visualization attributes ------------------------------------------------
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    visAttributes->SetVisibility(true);
    worldLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    
    G4VisAttributes* visAttributesAir = new G4VisAttributes(G4Colour(.0,.0,1.0));
    visAttributesAir->SetVisibility(true);
    airBoxLogical->SetVisAttributes(visAttributesAir);
    fVisAttributes.push_back(visAttributesAir);
    
    visAttributes = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
    visAttributes->SetVisibility(true);
    teflonTubeLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    

    visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    visAttributes->SetVisibility(true);
    fibbersLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    //visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    //visAttributes->SetVisibility(true);
    //sourceLogical->SetVisAttributes(visAttributes);
    //fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    visAttributes->SetVisibility(true);
    siliconeLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
 
    visAttributes = new G4VisAttributes(G4Colour(0.5,0.5,0.0));
    visAttributes->SetVisibility(true);
    pmtLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour(0.0,0.5,0.5));
    visAttributes->SetVisibility(true);
    photocathodeLogical->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
 
    // return the world physical volume ----------------------------------------
    
    return worldPhysical;
}



void DetectorConstruction::ConstructSDandField()
{
    //// Sensitive detectors
    G4String fibberSDname = "/fibberSD";
    fibberSD* aFibberSD = new fibberSD(fibberSDname,"fibberHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aFibberSD);
    //// Setting aFibberSD to all logical volumes with the same name
    //// of "fibberLogical".

    SetSensitiveDetector("fibbersLogical", aFibberSD, true);
   
    //PMT SD
   
    PMTSD* pmtSD = new PMTSD("pmtSD","HitsCollectionPMTs");
    G4SDManager::GetSDMpointer()->AddNewDetector(pmtSD);
    SetSensitiveDetector("photocathodeLogical", pmtSD,true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//G4VPhysicalVolume* DetectorConstruction::GetWaterPV(G4int number){
    //return sourcePhysical[number];
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......	}

G4VPhysicalVolume* DetectorConstruction::GetFibbersPV(G4int number){
    return fibbersPhysical[number];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......	
	
G4int DetectorConstruction::GetNFibbers()
{
    return nTotalFibbers;
}	


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//G4double DetectorConstruction::GetSourceRadius()
//{
  //return rSource;
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//G4double DetectorConstruction::GetSourceLength()
//{
    //return halfLength;
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double DetectorConstruction::GetFibberLength()
{
    return 2*fibberHalfLength;
}	

G4double DetectorConstruction::GetFibberRadius()
{
    return rInt;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4Material* DetectorConstruction::FindMaterial(G4String name) {
    G4Material* material = G4Material::GetMaterial(name,true);
    return material;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
