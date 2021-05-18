//******************************************************************************
// PrimaryGeneratorAction.cc
//
// 1.00 JMV, LLNL, Jan-2007:  First version.
//******************************************************************************
//

#include <iomanip>
#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "Analysis.hh"
#include "DetectorConstruction.hh"

using namespace std;

#include "G4Event.hh"

//----------------------------------------------------------------------------//
PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* detectorConstruction):
fParticleDefinition(0), fParticleEnergy(0), fParticlePosX(0),fParticlePosY(0),fParticlePosZ(0),fParticleMomentumDirectionU(0), fParticleMomentumDirectionV(0),
fParticleMomentumDirectionW(0),fParticleTime(0), fDetConstruction(detectorConstruction)
{
    // define a particle gun
  
  
    const char  *inputfile="setupCry.file";
  
    //G4cout<<"\n AQUI GENERATOR ACTION\n "<<G4endl;
    particleGun = new G4ParticleGun();

    // Read the cry input file
    std::ifstream inputFile;
    inputFile.open(inputfile,std::ios::in);
  
    if (!inputFile.good()){
        G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
        G4cout<<"File is in the folder?"<<G4endl;
        G4cout<<"EXITING"<<G4endl;

        exit(0);
    }
  
    char buffer[1000];

    if (inputFile.fail()) {
        if( *inputfile !=0)  //....only complain if a filename was given
            G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= " << inputfile << G4endl;
        InputState=-1;
    }
    else{
        std::string setupString("");
        while ( !inputFile.getline(buffer,1000).eof()) {
            setupString.append(buffer);
            setupString.append(" ");
        }

        CRYSetup *setup=new CRYSetup(setupString,"/usr/local/cry_v1.7/data");

        gen = new CRYGenerator(setup);

        // set random number generator
        RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
        setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
        InputState=0;
    }
    // create a vector to store the CRY particle properties
    vect=new std::vector<CRYParticle*>;

    // Create the table containing all particle names
    particleTable = G4ParticleTable::GetParticleTable();

    // Create the messenger file
    gunMessenger = new PrimaryGeneratorMessenger(this);
}

//----------------------------------------------------------------------------//
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::InputCRY()
{
    InputState=1;
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::UpdateCRY(std::string* MessInput)
{
    CRYSetup *setup=new CRYSetup(*MessInput,"/usr/local/cry_v1.7/data");

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState=0;

}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::CRYFromFile(G4String newValue)
{
    // Read the cry input file
    std::ifstream inputFile;
    inputFile.open(newValue,std::ios::in);
    char buffer[1000];

    if (inputFile.fail()) {
        G4cout << "Failed to open input file " << newValue << G4endl;
        G4cout << "Make sure to define the cry library on the command line" << G4endl;
        InputState=-1;
    }
    else{
        std::string setupString("");
        while ( !inputFile.getline(buffer,1000).eof()) {
            setupString.append(buffer);
            setupString.append(" ");
        }

        CRYSetup *setup=new CRYSetup(setupString,"/usr/local/cry_v1.7/data");

        gen = new CRYGenerator(setup);

        //set random number generator
        RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),&CLHEP::HepRandomEngine::flat);
        setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
        InputState=0;
    }
}

//----------------------------------------------------------------------------//
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
    if (InputState != 0) {
        G4String* str = new G4String("CRY library was not successfully initialized");
        //G4Exception(*str);
        G4Exception("PrimaryGeneratorAction", "1",
                RunMustBeAborted, *str);
    }

    G4String particleName;
    vect->clear();
    gen->genEvent(vect);

    //....debug output
    G4cout << "\nEvent=" << anEvent->GetEventID() << " "
         << "CRY generated nparticles=" << vect->size()
         << G4endl;

    for ( unsigned j=0; j<vect->size(); j++) {
        particleName=CRYUtils::partName((*vect)[j]->id());

        //....debug output
        cout << "  "          << particleName << " "
         << "charge="      << (*vect)[j]->charge() << " "
         << setprecision(4)
         << "energy (MeV)=" << (*vect)[j]->ke()*MeV << " "
         << "pos (m)"
         << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
         << " " << "direction cosines "
         << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
         << " " << endl;


	fParticleDefinition=particleTable->FindParticle((*vect)[j]->PDGid());
	fParticleEnergy=(*vect)[j]->ke()*MeV;
	fParticlePosX=(*vect)[j]->x()*m;
	fParticlePosY=(*vect)[j]->y()*m;
	fParticlePosZ=(*vect)[j]->z()*m;
	fParticleMomentumDirectionU=(*vect)[j]->u();
	fParticleMomentumDirectionV=(*vect)[j]->v();
	fParticleMomentumDirectionW=(*vect)[j]->w();
	fParticleTime=(*vect)[j]->t();

        //G4cout<<"AQUI Z POS "<<fParticlePosZ<<G4endl;
        //fParticlePosZ=fParticlePosZ+35*cm;
        //G4cout<<"AQUI Z POS "<<fParticlePosZ<<G4endl;
        fParticlePosZ=fDetConstruction->GetHeightCosmics();
        G4cout << "AQUI Z POS " << fParticlePosZ << G4endl;

        particleGun->SetParticleDefinition(fParticleDefinition);
        particleGun->SetParticleEnergy(fParticleEnergy);
        particleGun->SetParticlePosition(G4ThreeVector(fParticlePosX,fParticlePosY,fParticlePosZ));
        particleGun->SetParticleMomentumDirection(G4ThreeVector(fParticleMomentumDirectionU,fParticleMomentumDirectionV,fParticleMomentumDirectionW));
        //particleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));
        particleGun->SetParticleTime(fParticleTime);
        particleGun->GeneratePrimaryVertex(anEvent);
   
        G4int dummyInt=-1;
        // get analysis manager
        auto analysisManager = G4AnalysisManager::Instance();

        analysisManager->FillNtupleDColumn(0,2, fParticleEnergy);
        analysisManager->FillNtupleDColumn(0,3, fParticlePosX);
        analysisManager->FillNtupleDColumn(0,4, fParticlePosY);
        analysisManager->FillNtupleDColumn(0,5, fParticlePosZ);
        analysisManager->FillNtupleDColumn(0,6, fParticleTime);
        analysisManager->FillNtupleIColumn(0,7, dummyInt);
        analysisManager->FillNtupleIColumn(0,8, dummyInt);
        analysisManager->FillNtupleIColumn(0,18, fParticleDefinition->GetPDGEncoding());
        analysisManager->FillNtupleDColumn(0,19, fParticleMomentumDirectionU);
        analysisManager->FillNtupleDColumn(0,20, fParticleMomentumDirectionV);
        analysisManager->FillNtupleDColumn(0,21, fParticleMomentumDirectionW);

        analysisManager->FillNtupleDColumn(3,2, fParticleEnergy);
        analysisManager->FillNtupleDColumn(3,3, fParticlePosX);
        analysisManager->FillNtupleDColumn(3,4, fParticlePosY);
        analysisManager->FillNtupleDColumn(3,5, fParticlePosZ);
        analysisManager->FillNtupleDColumn(3,6, fParticleTime);
        analysisManager->FillNtupleIColumn(3,7, dummyInt);
        analysisManager->FillNtupleIColumn(3,8, dummyInt);
        analysisManager->FillNtupleIColumn(3,23, fParticleDefinition->GetPDGEncoding());
        analysisManager->FillNtupleDColumn(3,24, fParticleMomentumDirectionU);
        analysisManager->FillNtupleDColumn(3,25, fParticleMomentumDirectionV);
        analysisManager->FillNtupleDColumn(3,26, fParticleMomentumDirectionW);

    
        delete (*vect)[j];
    }
}
