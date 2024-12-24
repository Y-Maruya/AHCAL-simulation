#include "EcalUnitSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ParticleTypes.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"

namespace SimCalModule
{
    EcalUnitSD::EcalUnitSD(G4String name)
        : G4VSensitiveDetector(name), fEcalUnitHitsCollection(nullptr), fHitCID(-1), fHitIndex(0), detector(nullptr)
    {
        detector = static_cast<const DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        CellIDDecoder.clear();
        collectionName.insert("EcalUnitHitsCollection");
    }

    EcalUnitSD::~EcalUnitSD()
    {
    }

    void EcalUnitSD::Initialize(G4HCofThisEvent *hitsCE)
    {
        fEcalUnitHitsCollection = new EcalUnitHitsCollection(SensitiveDetectorName, collectionName[0]);
        if (fHitCID < 0)
        {
            fHitCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
        }
        hitsCE->AddHitsCollection(fHitCID, fEcalUnitHitsCollection);
        fHitIndex = 0;
        CellIDDecoder.clear();
    }

    G4bool EcalUnitSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
    {
        G4double edep = aStep->GetTotalEnergyDeposit();
        if (edep == 0)
            return false;
        if (aStep->GetTrack()->GetGlobalTime() > detector->GetEcalStepTimeLimit())
            return false;
        G4StepPoint *thePrePoint = aStep->GetPreStepPoint();
        G4StepPoint *thePostPoint = aStep->GetPostStepPoint();
        G4int copyNo = thePrePoint->GetTouchableHandle()->GetCopyNumber(2);
        
        EcalUnitHit *hit = nullptr;
        
        if (CellIDDecoder.find(copyNo) != CellIDDecoder.end())
        {
            hit = (*fEcalUnitHitsCollection)[CellIDDecoder[copyNo]];
            hit->AddEdepCell(aStep->GetTotalEnergyDeposit());
            hit->AddVisibleEdepCell(BirksAttenuation(aStep));
            hit->AddStepsCell(1);
            if (hit->GetEdepTemp() < aStep->GetTotalEnergyDeposit())
            {
                hit->SetTimeCell((thePrePoint->GetGlobalTime() + thePostPoint->GetGlobalTime()) / 2.);
                hit->SetEdepTemp(aStep->GetTotalEnergyDeposit());
            }
        }
        else
        {
            hit = new EcalUnitHit(copyNo);
            hit->SetEdepCell(aStep->GetTotalEnergyDeposit());
            hit->SetVisibleEdepCell(BirksAttenuation(aStep));
            hit->SetEdepTemp(aStep->GetTotalEnergyDeposit());
            hit->SetTimeCell((thePrePoint->GetGlobalTime() + thePostPoint->GetGlobalTime()) / 2.);
            hit->SetToaCell(aStep->GetTrack()->GetGlobalTime());
            hit->SetStepsCell(1);
            fEcalUnitHitsCollection->insert(hit);
            CellIDDecoder.insert(std::make_pair(copyNo, fHitIndex));
            fHitIndex++;
        }

        return true;
    }

    G4double EcalUnitSD::BirksAttenuation(const G4Step *aStep)
    {
        // Example of Birk attenuation law in organic scintillators.
        // adapted from Geant3 PHYS337. See MIN 80 (1970) 239-244
        G4Material *material = aStep->GetTrack()->GetMaterial();
        G4double birk1 = material->GetIonisation()->GetBirksConstant();
        G4double destep = aStep->GetTotalEnergyDeposit();
        G4double stepl = aStep->GetStepLength();
        G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
        G4double response = destep;
        if (birk1 * destep * stepl * charge != 0.)
        {
            response = destep / (1. + birk1 * destep / stepl);
        }
        return response;
    }
}