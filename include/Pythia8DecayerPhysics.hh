#ifndef PYTHIA8DecayerPhysics_H
#define PYTHIA8DecayerPhysics_H

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class G4Decay;

class Pythia8DecayerPhysics : public G4VPhysicsConstructor
{
  public:
    Pythia8DecayerPhysics(G4int verb = 1);
    virtual ~Pythia8DecayerPhysics();

  protected:
    // methods
    // construct particle and physics
    virtual void ConstructParticle();
    virtual void ConstructProcess();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif