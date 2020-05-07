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
// $Id: Materials.cc 82854 2014-07-14 09:08:25Z gcosmo $
//
/// \file optical/wls/src/Materials.cc
/// \brief Implementation of the Materials class
//
//
#include "Materials.hh"

#include "G4SystemOfUnits.hh"

Materials* Materials::fInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Materials::Materials()
{
  fNistMan = G4NistManager::Instance();

  fNistMan->SetVerbose(2);

  CreateMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Materials::~Materials()
{
  delete    fWater;
  delete    fPMMA;
  delete    fPolystyrene;
  delete    fTeflon;
  delete    fSilicone;
  delete fGlass;
  delete fPhotocathode;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Materials* Materials::GetInstance()
{
  if (fInstance == 0)
    {
      fInstance = new Materials();
    }
  return fInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* Materials::GetMaterial(const G4String material)
{
  G4Material* mat =  fNistMan->FindOrBuildMaterial(material);

  if (!mat) mat = G4Material::GetMaterial(material);
  if (!mat) {
     std::ostringstream o;
     o << "Material " << material << " not found!";
     G4Exception("Materials::GetMaterial","",
                 FatalException,o.str().c_str());
  }

  return mat;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Materials::CreateMaterials()
{
  G4double density;
  std::vector<G4int> natoms;
  std::vector<G4double> fractionMass;
  std::vector<G4String> elements;

  // Materials Definitions
  // =====================

  //--------------------------------------------------
  // Vacuum
  //--------------------------------------------------

  //fNistMan->FindOrBuildMaterial("G4_Galactic");

  //--------------------------------------------------
  // Water
  //--------------------------------------------------

  fWater = fNistMan->FindOrBuildMaterial("G4_WATER");
  
  
    //--------------------------------------------------
  // TEFLON
  //--------------------------------------------------

  fTeflon = fNistMan->FindOrBuildMaterial("G4_TEFLON");

  //--------------------------------------------------
// PMMA
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(5);
  elements.push_back("H");     natoms.push_back(8);
  elements.push_back("O");     natoms.push_back(2);

  density = 1.190*g/cm3;

  fPMMA = fNistMan->
          ConstructNewMaterial("PMMA", elements, natoms, density);

  elements.clear();
  natoms.clear();

  
  //--------------------------------------------------
  // Polystyrene
  //--------------------------------------------------
  
   fPolystyrene =fNistMan->FindOrBuildMaterial("G4_POLYSTYRENE");

  //--------------------------------------------------
  // Silicone (Template for Optical Grease)
  //--------------------------------------------------

  elements.push_back("C");     natoms.push_back(2);
  elements.push_back("H");     natoms.push_back(6);
  
  density = 1.060*g/cm3;

  fSilicone = fNistMan->
          ConstructNewMaterial("Silicone", elements, natoms, density);

  elements.clear();
  natoms.clear();
  
  
  
  //--------------------------------------------------
  // glass for PMT window
  //

  elements.push_back("Si");     natoms.push_back(1);
  elements.push_back("O");     natoms.push_back(2);

  density = 1.032*g/cm3;

  fGlass= fNistMan->
          ConstructNewMaterial("glass", elements, natoms, density);

  elements.clear();
  natoms.clear();
  
  
   //--------------------------------------------------
  // Photocathode
  //

  elements.push_back("Sb");     natoms.push_back(1);
  elements.push_back("Rb");     natoms.push_back(1);
	elements.push_back("Cs");     natoms.push_back(1);
	
	density= 4.28*g/cm3;

  fPhotocathode= fNistMan->
          ConstructNewMaterial("photocathode", elements, natoms, density);

  elements.clear();
  natoms.clear();
  
  

  
  
  // ------------ Generate & Add Material Properties Table ------------
  //

  
  
  // Water
  
  
  
  G4double photonEnergyWater[] =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
  
 const G4int nEntriesWater = sizeof(photonEnergyWater)/sizeof(G4double); 
  
  
//
  G4double refractiveIndexWater[] =
            { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
              1.346,  1.3465, 1.347,  1.3475, 1.348,
              1.3485, 1.3492, 1.35,   1.3505, 1.351,
              1.3518, 1.3522, 1.3530, 1.3535, 1.354,
              1.3545, 1.355,  1.3555, 1.356,  1.3568,
              1.3572, 1.358,  1.3585, 1.359,  1.3595,
              1.36,   1.3608};

  assert(sizeof(refractiveIndexWater) == sizeof(photonEnergyWater));

  G4double absorptionWater[] =
           {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
           15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
           45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
           52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
           30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
           17.500*m, 14.500*m };

  assert(sizeof(absorptionWater) == sizeof(photonEnergyWater));

  G4MaterialPropertiesTable* mptWater = new G4MaterialPropertiesTable();

  mptWater->AddProperty("RINDEX",       photonEnergyWater, refractiveIndexWater,nEntriesWater)
        ->SetSpline(true);
  mptWater->AddProperty("ABSLENGTH",    photonEnergyWater, absorptionWater,     nEntriesWater)
        ->SetSpline(true);
 
 
  G4double energy_water[] = {
     1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
     1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
     1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
     1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
     1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
     2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
     2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
     2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
     2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
     2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
     3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
     3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
     3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
     4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
     5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };

  const G4int numentries_water = sizeof(energy_water)/sizeof(G4double);

  //assume 100 times larger than the rayleigh scattering for now.
  G4double mie_water[] = {
     167024.4*m, 158726.7*m, 150742  *m,
     143062.5*m, 135680.2*m, 128587.4*m,
     121776.3*m, 115239.5*m, 108969.5*m,
     102958.8*m, 97200.35*m, 91686.86*m,
     86411.33*m, 81366.79*m, 76546.42*m,
     71943.46*m, 67551.29*m, 63363.36*m,
     59373.25*m, 55574.61*m, 51961.24*m,
     48527.00*m, 45265.87*m, 42171.94*m,
     39239.39*m, 36462.50*m, 33835.68*m,
     31353.41*m, 29010.30*m, 26801.03*m,
     24720.42*m, 22763.36*m, 20924.88*m,
     19200.07*m, 17584.16*m, 16072.45*m,
     14660.38*m, 13343.46*m, 12117.33*m,
     10977.70*m, 9920.416*m, 8941.407*m,
     8036.711*m, 7202.470*m, 6434.927*m,
     5730.429*m, 5085.425*m, 4496.467*m,
     3960.210*m, 3473.413*m, 3032.937*m,
     2635.746*m, 2278.907*m, 1959.588*m,
     1675.064*m, 1422.710*m, 1200.004*m,
     1004.528*m, 833.9666*m, 686.1063*m
  };

  assert(sizeof(mie_water) == sizeof(energy_water));

  // gforward, gbackward, forward backward ratio
  G4double mie_water_const[3]={0.99,0.99,0.8};

  mptWater->AddProperty("MIEHG",energy_water,mie_water,numentries_water)
        ->SetSpline(true);
  mptWater->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
  mptWater->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
  mptWater->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);

  G4cout << "Water G4MaterialPropertiesTable" << G4endl;
  //mptWater->DumpTable();

  fWater->SetMaterialPropertiesTable(mptWater);

  // Set the Birks Constant for the Water scintillator

  //water->GetIonisation()->SetBirksConstant(0.126*mm/MeV);


  //--------------------------------------------------
  //  Polystyrene
  //--------------------------------------------------

G4double photonEnergyGeneral[] =
{2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};




G4double photonEnergyPS[]
	{ 2.22*eV,2.29*eV,2.31*eV,2.35*eV,2.41*eV,2.44*eV,2.52*eV,2.53*eV,2.63*eV,
	2.65*eV,2.71*eV,2.73*eV,2.82*eV,2.87*eV,2.93*eV,2.99*eV,3.04*eV,3.13*eV,3.17*eV};


  const G4int nEntriesPS = sizeof(photonEnergyPS)/sizeof(G4double);

  const G4int nEntriesGeneral = sizeof(photonEnergyGeneral)/sizeof(G4double);



  G4double refractiveIndexPS[] =
  {1.594,1.597,1.597,1.598,1.599,1.600,1.604,1.604,1.606,
	  1.607,1.611,1.612,1.614,1.616,1.618,1.620,1.622,1.625};
  


  assert(sizeof(refractiveIndexPS) == sizeof(photonEnergyPS));

  

G4double photonEnergyAbsorption[] =
{2.24*eV,2.27*eV,2.28*eV,2.30*eV,2.32*eV,2.33*eV,2.38*eV,2.38*eV,2.41*eV,2.44*eV,2.46*eV,2.48*eV,2.50*eV,2.53*eV,
2.55*eV,2.57*eV,2.60*eV,2.60*eV,2.65*eV,2.65*eV,2.69*eV,2.71*eV,2.73*eV,2.77*eV,2.78*eV,2.82*eV,2.83*eV,2.87*eV,
2.90*eV,2.92*eV,2.97*eV,2.97*eV,3.04*eV,3.06*eV,3.09*eV,3.09*eV,3.10*eV};


  const G4int nEntriesAbsorption = sizeof(photonEnergyAbsorption)/sizeof(G4double);



G4double absPS[] =
  {4.56*m,4.54*m,4.53*m,4.50*m,4.46*m,4.46*m,4.52*m,4.52*m,4.44*m,4.38*m,4.33*m,4.26*m,4.22*m,4.09*m,4.00*m,
3.91*m,3.79*m,3.78*m,3.65*m,3.64*m,3.53*m,3.45*m,3.38*m,3.24*m,3.20*m,3.03*m,3.00*m,2.83*m,2.71*m,2.61*m,
2.43*m,2.42*m,2.20*m,1.93*m,1.71*m,1.66*m,1.56*m};



  assert(sizeof(absPS) == sizeof(photonEnergyAbsorption));


//From Saint gobain brochure

 G4double photoEmissionEnergy[]=
 {2.26*eV,2.29*eV,2.31*eV,2.34*eV,2.36*eV,2.38*eV,2.40*eV,2.42*eV,2.45*eV,2.47*eV,
2.50*eV,2.53*eV,2.55*eV,2.58*eV,2.59*eV,2.60*eV,2.61*eV,2.62*eV,2.62*eV,2.63*eV,
2.64*eV,2.65*eV,2.66*eV,2.66*eV,2.67*eV,2.68*eV,2.69*eV,2.70*eV,2.71*eV,2.72*eV,
2.73*eV,2.74*eV,2.76*eV,2.77*eV,2.78*eV,2.79*eV,2.80*eV,2.80*eV,2.81*eV,2.82*eV,
2.82*eV,2.83*eV,2.83*eV,2.84*eV,2.84*eV,2.85*eV,2.86*eV,2.87*eV,2.88*eV,2.89*eV,
2.89*eV,2.90*eV,2.90*eV,2.91*eV,2.92*eV,2.94*eV,2.94*eV,2.95*eV,2.96*eV,2.96*eV,
2.96*eV,2.97*eV,2.97*eV,2.98*eV,2.98*eV,2.98*eV,2.99*eV,2.99*eV,2.99*eV,3.00*eV,
3.00*eV,3.00*eV,3.01*eV,3.01*eV,3.01*eV,3.02*eV,3.04*eV,3.05*eV,3.06*eV,3.07*eV,3.09*eV};



  G4double scintilFast[] =
  {0.00,0.00,0.00,0.01,0.01,0.03,0.04,0.05,0.06,0.09,
0.11,0.13,0.16,0.18,0.21,0.23,0.26,0.28,0.30,0.32,
0.34,0.36,0.38,0.41,0.44,0.47,0.50,0.52,0.55,0.57,
0.59,0.61,0.65,0.67,0.69,0.71,0.73,0.76,0.79,0.82,
0.86,0.88,0.92,0.94,0.97,0.98,1.00,0.98,0.96,0.94,
0.91,0.89,0.86,0.84,0.81,0.76,0.72,0.68,0.65,0.61,
0.58,0.54,0.50,0.47,0.44,0.44,0.37,0.34,0.32,0.28,
0.26,0.24,0.21,0.18,0.16,0.14,0.10,0.05,0.03,0.01,0.00};

  assert(sizeof(scintilFast) == sizeof(photoEmissionEnergy));
  
    const G4int nEntriesFastComp = sizeof(photoEmissionEnergy)/sizeof(G4double);
  

  // Add entries into properties table
  G4MaterialPropertiesTable* mptPolystyrene = new G4MaterialPropertiesTable();
  mptPolystyrene->AddProperty("RINDEX",photonEnergyPS,refractiveIndexPS,nEntriesPS);//checked
  mptPolystyrene->AddProperty("ABSLENGTH",photonEnergyAbsorption,absPS,nEntriesAbsorption);//checked
  mptPolystyrene->
               AddProperty("FASTCOMPONENT",photoEmissionEnergy, scintilFast,nEntriesFastComp);//checked
  mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD",8./keV);//checked
  mptPolystyrene->AddConstProperty("RESOLUTIONSCALE",0.0);//TO MEASURE
  mptPolystyrene->AddConstProperty("FASTTIMECONSTANT", 2.7*ns);	//checked
 
  fPolystyrene->SetMaterialPropertiesTable(mptPolystyrene);

  // Set the Birks Constant for the Polystyrene scintillator

  fPolystyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);//checked
  
  
    //--------------------------------------------------
  //  PMMA for EndCaps
  //--------------------------------------------------

  G4double refractiveIndexPMMA[] =
  { 1.493,1.494,1.495,1.495,1.496,1.496,1.497,1.497,1.499,
	  1.500,1.501,1.501,1.503,1.503,1.504,1.505,1.506,1.508};

  assert(sizeof(refractiveIndexPMMA) == sizeof(photonEnergyPS));

  G4double absPMMA[] =
  {4.78*m,4.78*m,4.79*m,4.79*m,4.79*m,4.78*m,4.78*m,4.78*m,4.79*m,4.78*m,4.77*m,4.76*m,4.77*m,4.78*m,
4.79*m,4.80*m,4.80*m,4.80*m,4.78*m,4.78*m,4.77*m,4.77*m,4.77*m,4.77*m,4.76*m,4.74*m,4.73*m,4.72*m,
4.71*m,4.70*m,4.67*m,4.67*m,4.64*m,4.63*m,4.60*m,4.60*m,4.59*m};

  assert(sizeof(absPMMA) == sizeof(photonEnergyAbsorption));


 
  // Add entries into properties table
  G4MaterialPropertiesTable* mptPMMA = new G4MaterialPropertiesTable();
  mptPMMA->
           AddProperty("RINDEX",photonEnergyPS,refractiveIndexPMMA,nEntriesPS);//checked
  mptPMMA->AddProperty("ABSLENGTH",photonEnergyAbsorption,absPMMA,nEntriesAbsorption);//checked

  fPMMA->SetMaterialPropertiesTable(mptPMMA);
  
  
 //--------------------------------------------------
  //  Teflontube
  //-------------------------------------------------- 
  
  G4double photonEnergyTeflon[]
	{ 2.16*eV,2.30*eV,2.39*eV,2.56*eV,2.72*eV,2.91*eV,3.19*eV,3.50*eV,3.74*eV,4.22*eV};


  const G4int nEntriesTeflon = sizeof(photonEnergyTeflon)/sizeof(G4double);
  
  
  
    G4double reflectivityPTFE[] =
  {0.91,0.92,0.92,0.92,0.93,0.93,0.93,0.92,0.92,0.90};

  assert(sizeof(reflectivityPTFE) == sizeof(photonEnergyTeflon));
  G4MaterialPropertiesTable* mptPTFE = new G4MaterialPropertiesTable();
  
  mptPTFE->
           AddProperty("REFLECTIVITY",photonEnergyTeflon ,reflectivityPTFE,nEntriesTeflon);//checked
  
  fTeflon->SetMaterialPropertiesTable(mptPTFE);
  
  
  //--------------------------------------------------
  // Silicone
  //--------------------------------------------------

   G4double refractiveIndexSilicone[] =
   { 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46};

   assert(sizeof(refractiveIndexSilicone) == sizeof(photonEnergyGeneral));

  // Add entries into properties table
  G4MaterialPropertiesTable* mptSilicone = new G4MaterialPropertiesTable();
  mptSilicone->
           AddProperty("RINDEX",photonEnergyGeneral,refractiveIndexSilicone,nEntriesGeneral);
  //mptSilicone->AddProperty("ABSLENGTH",photonEnergy,absClad,nEntries);

  fSilicone->SetMaterialPropertiesTable(mptSilicone);
  
   //--------------------------------------------------
  // Borosilicate Glass
  //--------------------------------------------------
  
    G4double photonEnergyBorosilicate[]
	{2.21*eV,2.27*eV,2.35*eV,2.43*eV,2.51*eV,2.61*eV,2.69*eV,2.78*eV,2.87*eV,2.98*eV,3.09*eV,3.20*eV};


  const G4int nEntriesBorosilicate = sizeof(photonEnergyBorosilicate)/sizeof(G4double);
  
 
  
   G4double refractiveIndexGlass[]=
   { 1.48,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.49,1.50,1.50};
   
   
  assert(sizeof(refractiveIndexGlass) == sizeof(photonEnergyBorosilicate));
  
  
  G4double absGlass[] =
  {420*cm,420*cm,420*cm,420*cm,420*cm,420*cm,420*cm,420*cm,420*cm,420*cm,420*cm,420*cm};
  
  
  assert(sizeof(absGlass) == sizeof(photonEnergyBorosilicate));
  
  G4MaterialPropertiesTable *mptGlass = new G4MaterialPropertiesTable();
	mptGlass->AddProperty("ABSLENGTH",photonEnergyBorosilicate,absGlass,nEntriesBorosilicate);
	mptGlass->AddProperty("RINDEX",photonEnergyBorosilicate,refractiveIndexGlass,nEntriesBorosilicate);//Checked
  fGlass->SetMaterialPropertiesTable(mptGlass);
  
  
   //--------------------------------------------------
  // Photocathode
  //--------------------------------------------------
   
   
   G4double photonEnergyPMT[]
	{2.23*eV,2.25*eV,2.27*eV,2.29*eV,2.31*eV,2.33*eV,2.37*eV,2.40*eV,2.42*eV,2.46*eV,2.48*eV,2.51*eV,2.54*eV,
2.57*eV,2.61*eV,2.64*eV,2.67*eV,2.70*eV,2.74*eV,2.80*eV,2.83*eV,2.87*eV,2.92*eV,2.98*eV,3.05*eV,3.17*eV};


  const G4int nEntriesPMT = sizeof(photonEnergyPMT)/sizeof(G4double);
   

   
  G4double photocath_EFF[]=
    {0.07,0.07,0.08,0.09,0.09,0.10,0.11,0.12,0.13,0.14,0.14,0.15,0.16,
0.17,0.17,0.18,0.19,0.20,0.20,0.21,0.21,0.22,0.23,0.23,0.24,0.24}; //Enables 'detection' of photons
    
  assert(sizeof(photocath_EFF) == sizeof(photonEnergyPMT));


 G4double reflectivityPhotocathode[] =
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

  assert(sizeof(reflectivityPhotocathode) == sizeof(photonEnergyPMT));



 mptPhoto = new G4MaterialPropertiesTable();
  mptPhoto->AddProperty("EFFICIENCY",photonEnergyPMT,photocath_EFF,nEntriesPMT);//Checked
    mptPhoto-> AddProperty("REFLECTIVITY",photonEnergyPMT,reflectivityPhotocathode,nEntriesPMT);
  fPhotocathode->SetMaterialPropertiesTable(mptPhoto);

  

}
G4MaterialPropertiesTable* Materials::GetPhotoMPT(){
	return mptPhoto;
	
	}
