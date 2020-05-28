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
// $Id: vetoHit.hh 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file vetoHit.hh
/// \brief Definition of the vetoHit class

#ifndef vetoHit_h
#define vetoHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, vetoNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fVetoNB, fEdep, fPos

class vetoHit : public G4VHit
{
  public:
    vetoHit();
    vetoHit(const vetoHit&);
    virtual ~vetoHit();

    // operators
    const vetoHit& operator=(const vetoHit&);
    G4int operator==(const vetoHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetParticlePDG  (G4int particlePDG)      { fParticlePDG = particlePDG; };
    void SetParticleEnergy  (G4double particleEnergy)      { fParticleEnergy = particleEnergy; };
    void SetVetoNb(G4int vetonum)      { fVetoNb = vetonum; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetParticlePDG() const     { return fParticlePDG; };
    G4double GetParticleEnergy() const {return fParticleEnergy;};
    G4int GetVetoNb() const   { return fVetoNb; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

  private:

      G4int         fTrackID;
      G4int			fParticlePDG;
      G4int         fVetoNb;
      G4double      fEdep;
      G4ThreeVector fPos;
      G4double  fParticleEnergy;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<vetoHit> vetoHitsCollection;

extern G4ThreadLocal G4Allocator<vetoHit>* vetoHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* vetoHit::operator new(size_t)
{
  if(!vetoHitAllocator)
      vetoHitAllocator = new G4Allocator<vetoHit>;
  return (void *) vetoHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void vetoHit::operator delete(void *hit)
{
  vetoHitAllocator->FreeSingle((vetoHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
