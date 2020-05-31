//******************************************************************************
// PrimaryGeneratorAction.hh
//
// This class is a class derived from G4VUserPrimaryGeneratorAction for 
// constructing the process used to generate incident particles.
//
// 1.00 JMV, LLNL, JAN-2007:  First version.
//******************************************************************************
// 
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "CRYSetup.h"
#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYUtils.h"
#include "vector"
#include "RNGWrapper.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "G4ParticleDefinition.hh"

class G4Event;
class DetectorConstruction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction* detectorConstruction);
    ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);
    void InputCRY();
    void UpdateCRY(std::string* MessInput);
    void CRYFromFile(G4String newValue);

  private:
    std::vector<CRYParticle*> *vect; // vector of generated particles
    G4ParticleTable* particleTable;
    G4ParticleGun* particleGun;
    CRYGenerator* gen;
    G4int InputState;
    PrimaryGeneratorMessenger* gunMessenger;
    
    G4ParticleDefinition *fParticleDefinition;
	G4double fParticleEnergy;
	G4double fParticlePosX;
	G4double fParticlePosY;
	G4double fParticlePosZ;
	G4double fParticleMomentumDirectionU;
	G4double fParticleMomentumDirectionV;
	G4double fParticleMomentumDirectionW;
	G4double fParticleTime;

        DetectorConstruction* fDetConstruction;

};

#endif





