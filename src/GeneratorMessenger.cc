#include "PrimaryGenerator.hh"
#include "GeneratorMessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"
#include <iostream>
#include <fstream>
#include <string>

namespace SimCalModule
{
    GeneratorMessenger::GeneratorMessenger(PrimaryGenerator* primaryGenerator)
        : G4UImessenger(),
            fPrimaryGenerator(primaryGenerator),
            fDirectory(new G4UIdirectory("/generator/")),
            fInputFileName(new G4UIcmdWithAString("/generator/inputFileName", this)),
            fFixedPrimaryVertexPosition(new G4UIcmdWithABool("/generator/FixedPrimaryVertexPosition", this)),
            fPrimaryVertexPosition(new G4UIcmdWith3VectorAndUnit("/generator/PrimaryVertexPosition", this)),
            fCCNue(new G4UIcmdWithABool("/generator/CCNue", this)),
            fCCNumu(new G4UIcmdWithABool("/generator/CCNumu", this)),
            fCCNutau(new G4UIcmdWithABool("/generator/CCNutau", this)),
            fNC(new G4UIcmdWithABool("/generator/NC", this))
        {
        fInputFileName->SetGuidance("Set input file name");
        fInputFileName->SetParameterName("inputFileName", true,true);
        fInputFileName->AvailableForStates(G4State_PreInit, G4State_Idle);
        
        fFixedPrimaryVertexPosition->SetGuidance("Set fixed particle position or not");
        fFixedPrimaryVertexPosition->SetParameterName("FixedPrimaryVertexPosition", true,true);
        fFixedPrimaryVertexPosition->AvailableForStates(G4State_PreInit, G4State_Idle);
        
        fPrimaryVertexPosition->SetGuidance("Set vertex position");
        fPrimaryVertexPosition->SetParameterName("PrimaryVertexPositionX", "PrimaryVertexPositionY", "PrimaryVertexPositionZ", true,true);
        fPrimaryVertexPosition->SetDefaultUnit("cm");
        fPrimaryVertexPosition->SetUnitCandidates("um mm cm m");
        fPrimaryVertexPosition->AvailableForStates(G4State_PreInit, G4State_Idle);
        
        fCCNue->SetGuidance("Set CCNue");
        fCCNue->SetParameterName("CCNue", true,true);
        fCCNue->AvailableForStates(G4State_PreInit, G4State_Idle);

        fCCNumu->SetGuidance("Set CCNumu");
        fCCNumu->SetParameterName("CCNumu", true,true);
        fCCNumu->AvailableForStates(G4State_PreInit, G4State_Idle);

        fCCNutau->SetGuidance("Set CCNutau");
        fCCNutau->SetParameterName("CCNutau", true,true);
        fCCNutau->AvailableForStates(G4State_PreInit, G4State_Idle);

        fNC->SetGuidance("Set NC");
        fNC->SetParameterName("NC", true,true);
        fNC->AvailableForStates(G4State_PreInit, G4State_Idle);

        // Default values
        fPrimaryGenerator->SetInputFileName("faser3.10fbInv.1.gfaser.root");
        fPrimaryGenerator->SetFixedPrimaryVertexPosition(true);
        fPrimaryGenerator->SetPrimaryVertexPosition(G4ThreeVector(0.0,0.0,0.25*cm));
        fPrimaryGenerator->SetCCNue(true);
        fPrimaryGenerator->SetCCNumu(true);
        fPrimaryGenerator->SetCCNutau(true);
        fPrimaryGenerator->SetNC(true);
    }

    GeneratorMessenger::~GeneratorMessenger()
    {
        delete fInputFileName;
        delete fFixedPrimaryVertexPosition;
        delete fPrimaryVertexPosition;
        delete fCCNue;
        delete fCCNumu;
        delete fCCNutau;
        delete fNC;
        delete fDirectory;
    }

    void GeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
    {
        if(command == fInputFileName)
        {
            fPrimaryGenerator->SetInputFileName(newValue);
            std::string outputFileName = newValue.substr(0,newValue.find(".root"))+"_interaction.txt";
            std::ofstream outputfile(outputFileName,std::ios_base::out|std::ios_base::trunc);
            outputfile.close();
        }
        else if(command == fFixedPrimaryVertexPosition)
        {
            fPrimaryGenerator->SetFixedPrimaryVertexPosition(fFixedPrimaryVertexPosition->GetNewBoolValue(newValue));
        }
        else if(command == fPrimaryVertexPosition)
        {
            fPrimaryGenerator->SetPrimaryVertexPosition(fPrimaryVertexPosition->GetNew3VectorValue(newValue));
        }
        else if(command == fCCNue)
        {
            fPrimaryGenerator->SetCCNue(fCCNue->GetNewBoolValue(newValue));
        }
        else if(command == fCCNumu)
        {
            fPrimaryGenerator->SetCCNumu(fCCNumu->GetNewBoolValue(newValue));
        }
        else if(command == fCCNutau)
        {
            fPrimaryGenerator->SetCCNutau(fCCNutau->GetNewBoolValue(newValue));
        }
        else if(command == fNC)
        {
            fPrimaryGenerator->SetNC(fNC->GetNewBoolValue(newValue));
        }
    }
}
