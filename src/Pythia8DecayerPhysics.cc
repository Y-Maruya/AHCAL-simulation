#include "Pythia8DecayerPhysics.hh"

#include "Pythia8Decayer.hh"

#include "G4Decay.hh"
#include "G4DecayTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// factory
//
#include "G4PhysicsConstructorFactory.hh"
//
// register it with contructor factory
//
G4_DECLARE_PHYSCONSTR_FACTORY(Pythia8DecayerPhysics);

bool IsTargetParticle(int pdgCode) {
   static const std::vector<int> targetPDGCodes = {
       15,   // Tau-
       521,  // B+
       511,  // B0
       541,  // Bc+
       531,  // Bs0
       411,  // D+
       421,  // D0
       431,  // Ds+
       443,  // J/Psi
       553,  // Upsilon
       5122, // Lambda_b
       4122, // Lambda_c
       5332, // Omega_b
       4322, // Omega_c
       4324, // omega_c0
       5232, // Xi_b-
       5132, // Xi_b0
       4232, // Xi_c+
       4132  // Xi_c0
   };

   return std::find(targetPDGCodes.begin(), targetPDGCodes.end(), std::abs(pdgCode)) != targetPDGCodes.end();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Pythia8DecayerPhysics::Pythia8DecayerPhysics(G4int) : G4VPhysicsConstructor("Pythia8DecayerPhysics") {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Pythia8DecayerPhysics::~Pythia8DecayerPhysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Pythia8DecayerPhysics::ConstructParticle()
{
  // Nothing needs to be done here
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Pythia8DecayerPhysics::ConstructProcess()
{
  // Adding external decayer to G4Decay process (per each thread).
  // G4Decay will use the external decayer if G4Decay process is
  // assigned to an unstable particle and that particle does not
  // have its decay table.

  // Loop over all particles instantiated and remove already-assigned
  // decay table for tau's and B+/- so that they will decay through
  // the external decayer (Pythia8).

  // NOTE: The extDecayer will be deleted in G4Decay destructor

  Pythia8Decayer* extDecayer = new Pythia8Decayer("Pythia8DecayerPhysics");
//   G4bool setOnce = true;

  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)())
   {    
      G4ParticleDefinition* particle = particleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();    
      G4ProcessVector* processVector = pmanager->GetProcessList();
      for ( size_t i=0; i<processVector->length(); ++i ) 
      {    
         G4Decay* decay = dynamic_cast<G4Decay*>((*processVector)[i]);
         if ( decay ) 
         {
            // remove native/existing decay table for
            // a)tau's 
            // b) B+/- 
            // and replace with external decayer
            if (  IsTargetParticle(particle->GetPDGEncoding()))
            {
               if ( particle->GetDecayTable() )
               {
                  delete particle->GetDecayTable();
                  particle->SetDecayTable(nullptr);
               }
               // decay->SetExtDecayer(extDecayer);
               // G4cout << "Setting ext decayer for: " 
               //  <<  particleIterator->value()->GetParticleName()
               //  << G4endl;
            }
            // now set external decayer to all particles 
            // that don't yet have a decay table
            if ( !particle->GetDecayTable() )
            {
               decay->SetExtDecayer(extDecayer);
               G4cout << "Setting ext decayer for: " 
                <<  particleIterator->value()->GetParticleName()
                << G4endl;
            }
         }
      }              
   }


  return;
}