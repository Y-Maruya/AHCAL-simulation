#ifndef GeneratorMessenger_h
#define GeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGenerator;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;

namespace SimCalModule
{
    class GeneratorMessenger : public G4UImessenger
    {
    public:
        GeneratorMessenger(PrimaryGenerator*);
        ~GeneratorMessenger() override;

        void SetNewValue(G4UIcommand*, G4String) override;

    private:
        PrimaryGenerator* fPrimaryGenerator;
        G4UIdirectory* fDirectory;
        G4UIcmdWithAString* fInputFileName;
        G4UIcmdWithABool* fFixedPrimaryVertexPosition;
        G4UIcmdWith3VectorAndUnit* fPrimaryVertexPosition;
        G4UIcmdWithABool* fCCNue;
        G4UIcmdWithABool* fCCNumu;
        G4UIcmdWithABool* fCCNutau;
        G4UIcmdWithABool* fNC;

    };
}

#endif
