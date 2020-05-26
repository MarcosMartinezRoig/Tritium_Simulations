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
// $Id: EventAction.hh 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Track.hh"
#include "G4PrimaryVertex.hh"
#include "globals.hh"
#include "Analysis.hh"

/// Event action class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers:
/// - fEnergyWater, fEnergyFibber, fTrackLWater, fTrackLFibber
/// which are collected step by step via the functions
/// - AddAbs(), AddGap()

class EventAction : public G4UserEventAction
{
  public:
    EventAction(G4int);
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
	
    void AddWater(G4double de, G4double dl);
    void AddFibbers(G4double de, G4double dl);
	G4int RunNumber;
	void FillTreeOptical(G4AnalysisManager*); 
	void FillTreeTritium(G4AnalysisManager*);


    
  private:
  

	G4int     fEventID;
    G4double  fEnergyWater;
    G4double  fEnergyFibber;
    G4double  fTrackLWater; 
    G4double  fTrackLFibber;
    G4int fSourceNumber;
    //G4int runNumber;
    G4int hitsCollID;
    G4int hitsCollIDfibbers;
     G4int pmtNumber;
      G4ThreeVector position;
		G4int trackID;
//G4int fDetection;
G4int nHitPMT0;
G4int nHitPMT1;
//G4int nDetectedPMT0;
//G4int nDetectedPMT1;
G4int fCoincidenceFlag;
G4double photonEnergy;  
G4int fSecondaryParticlePDG;
  G4double fSecondaryParticleEnergy;
};

// inline functions

inline void EventAction::AddWater(G4double de, G4double dl) {
  fEnergyWater += de; 
  fTrackLWater += dl;
}

inline void EventAction::AddFibbers(G4double de, G4double dl) {
  fEnergyFibber += de; 
  fTrackLFibber += dl;
}
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
