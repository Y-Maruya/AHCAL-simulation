#ifndef PrimaryGenerator_h
#define PrimaryGenerator_h 1

// #include "GeneratorMessenger.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "RunAction.hh"
#include "GeneratorMessenger.hh"
#include "globals.hh"
class G4Event;
class G4ParticleGun;
// class PrimaryGenerator;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;

namespace SimCalModule
{
    class RunAction;
  //------------------------------------------------------------------------------
    class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
  //------------------------------------------------------------------------------
  {
    public:
      PrimaryGenerator();    
    ~PrimaryGenerator();

    public:
      virtual void GeneratePrimaries(G4Event*);
      void SetInputFileName(G4String name);
      G4String GetInputFileName() const; // Add const
      void SetFixedPrimaryVertexPosition(G4bool fixed);
      void SetPrimaryVertexPosition(G4ThreeVector position);
      void SetCCNue(G4bool ccnue);
      void SetCCNumu(G4bool ccnumu);
      void SetCCNutau(G4bool ccnutau);
      void SetNC(G4bool nc);
      void SetRunAction(RunAction* runaction){fRunAction = runaction;}
      G4int GetftagNulabel() const; // Add const
      G4double GetPrimaryEnergy() const; // Add const
      G4ThreeVector GetInteractionPlace() const; // Add const

    private:
      GeneratorMessenger* fMessenger;
      RunAction* fRunAction;
      G4ParticleGun* fpParticleGun;
      G4String fInputFileName;
      G4bool fFixedPrimaryVertexPosition;
      G4ThreeVector fPrimaryVertexPosition;
      G4bool fNC;
      G4bool fCCNue;
      G4bool fCCNumu;
      G4bool fCCNutau;
      G4int ftagNulabel;
      G4double primaryenergy;

  };
}

#endif
