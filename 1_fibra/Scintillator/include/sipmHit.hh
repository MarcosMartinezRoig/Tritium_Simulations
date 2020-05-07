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
// $Id: sipmHit.hh 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file sipmHit.hh
/// \brief Definition of the sipmHit class

#ifndef sipmHit_h
#define sipmHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
//#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, sipmNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fsipmNB, fEdep, fPos

class sipmHit : public G4VHit
{
  public:
    sipmHit();
    sipmHit(const sipmHit&);
    virtual ~sipmHit();

    // operators
    const sipmHit& operator=(const sipmHit&);
    G4int operator==(const sipmHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    //virtual void Draw();
    //virtual void Print();

    // Set methods
   void SetTrackID  (G4int track)      { fTrackID = track; };
   void SetsipmNb(G4int fibb)      { fsipmNb = fibb; };
   void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
   void SetEnergy(G4double photonEnergy)      { fEnergy = photonEnergy; };
   //void SetDetection (G4int detFlag){fDet=detFlag;}

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetsipmNb() const   { return fsipmNb; };
    G4ThreeVector GetPos() const { return fPos; };
    G4double GetEnergy() const   { return fEnergy; };
    //G4int GetDetection () const{ return fDet;}
  private:

      G4int         fTrackID;
      G4int         fsipmNb;
      G4ThreeVector fPos;
      G4double fEnergy;
      //G4int fDet;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<sipmHit> sipmHitsCollection;

extern G4ThreadLocal G4Allocator<sipmHit>* sipmHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* sipmHit::operator new(size_t)
{
  if(!sipmHitAllocator)
      sipmHitAllocator = new G4Allocator<sipmHit>;
  return (void *) sipmHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void sipmHit::operator delete(void *hit)
{
  sipmHitAllocator->FreeSingle((sipmHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
