#include "EventAction.hh"
#include "PrimaryGenerator.hh"
#include "DetectorConstruction.hh"
#include "EcalUnitSD.hh"
#include "HcalUnitSD.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4Track.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"


namespace SimCalModule
{
    EventAction::EventAction(RunAction *aRun)
        : G4UserEventAction(), fRunAction(aRun), fEcalCollID(-1), fHcalCollID(-1)
    {
    }

    EventAction::~EventAction()
    {
    }

    void EventAction::BeginOfEventAction(const G4Event *anEvent)
    {
        ResetEventData();

        EvtID = anEvent->GetEventID();
        if (EvtID % 100 == 0)
            G4cout << "Start to simulate the event " << EvtID << G4endl;

        const PrimaryGenerator *primary = static_cast<const PrimaryGenerator *>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
        ParticleEnergy = primary->GetPrimaryEnergy() / GeV;
        ftagNulabel = primary->GetftagNulabel();
        interaction_x = primary->GetInteractionPlace().x() / mm;
        interaction_y = primary->GetInteractionPlace().y() / mm;
        interaction_z = primary->GetInteractionPlace().z() / mm;

        const DetectorConstruction *detector = static_cast<const DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        if (fEcalCollID < 0 && detector->GetEcalModuleType() > 0)
        {
            auto sdManager = G4SDManager::GetSDMpointer();
            fEcalCollID = sdManager->GetCollectionID("EcalUnitHitsCollection");
        }

        if (fHcalCollID < 0 && detector->GetHcalModuleType() > 0)
        {
            auto sdManager = G4SDManager::GetSDMpointer();
            fHcalCollID = sdManager->GetCollectionID("HcalUnitHitsCollection");
        }
    }

    void EventAction::EndOfEventAction(const G4Event *anEvent)
    {
        G4HCofThisEvent *hitsCE = anEvent->GetHCofThisEvent();
        EcalUnitHitsCollection *EcalUnitHC = nullptr;
        HcalUnitHitsCollection *HcalUnitHC = nullptr;
        if (hitsCE && fEcalCollID >= 0)
            EcalUnitHC = (EcalUnitHitsCollection *)(hitsCE->GetHC(fEcalCollID));
        if (hitsCE && fHcalCollID >= 0)
            HcalUnitHC = (HcalUnitHitsCollection *)(hitsCE->GetHC(fHcalCollID));

        if (EcalUnitHC)
        {
            size_t n_hit = EcalUnitHC->entries();
            G4double fEdepCell;
            G4double fVisibleEdepCell;
            for (size_t i = 0; i < n_hit; i++)
            {
                fEdepCell = (*EcalUnitHC)[i]->GetEdepCell() / MeV;
                fVisibleEdepCell = (*EcalUnitHC)[i]->GetVisibleEdepCell() / MeV;
                EcalEdepSum += fEdepCell;
                EcalVisibleEdepSum += fVisibleEdepCell;
                vecEcalCellID.push_back((*EcalUnitHC)[i]->GetCellID());
                vecEcalStepsCell.push_back((*EcalUnitHC)[i]->GetStepsCell());
                vecEcalEdepCell.push_back(fEdepCell);
                vecEcalVisibleEdepCell.push_back(fVisibleEdepCell);
                vecEcalHitTimeCell.push_back((*EcalUnitHC)[i]->GetTimeCell() / ns);
                vecEcalToaCell.push_back((*EcalUnitHC)[i]->GetToaCell() / ns);
            }
            if (vecEcalEdepCell.size() > 0)
            {
                auto fMaxEnergy_it = std::max_element(vecEcalEdepCell.begin(), vecEcalEdepCell.end());
                EcalMaxEdepCell = *fMaxEnergy_it;
            }
        }

        if (HcalUnitHC)
        {
            size_t n_hit = HcalUnitHC->entries();
            G4double fEdepCell;
            G4double fVisibleEdepCell;
            for (size_t i = 0; i < n_hit; i++)
            {
                fEdepCell = (*HcalUnitHC)[i]->GetEdepCell() / MeV;
                fVisibleEdepCell = (*HcalUnitHC)[i]->GetVisibleEdepCell() / MeV;
                HcalEdepSum += fEdepCell;
                HcalVisibleEdepSum += fVisibleEdepCell;
                vecHcalCellID.push_back((*HcalUnitHC)[i]->GetCellID());
                vecHcalStepsCell.push_back((*HcalUnitHC)[i]->GetStepsCell());
                vecHcalEdepCell.push_back(fEdepCell);
                vecHcalVisibleEdepCell.push_back(fVisibleEdepCell);
                vecHcalHitTimeCell.push_back((*HcalUnitHC)[i]->GetTimeCell() / ns);
                vecHcalToaCell.push_back((*HcalUnitHC)[i]->GetToaCell() / ns);
            }
            if (vecHcalEdepCell.size() > 0)
            {
                auto fMaxEnergy_it = std::max_element(vecHcalEdepCell.begin(), vecHcalEdepCell.end());
                HcalMaxEdepCell = *fMaxEnergy_it;
            }
        }

        CaloEdepSum = EcalEdepSum + HcalEdepSum;
        CaloVisibleEdepSum = EcalVisibleEdepSum + HcalVisibleEdepSum;
        //TruthParticle
        G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
        if (trajectoryContainer) {
            for (size_t i = 0; i < trajectoryContainer->size(); ++i) {
                G4Trajectory* trajectory = static_cast<G4Trajectory*>((*trajectoryContainer)[i]);
                G4double mass = G4ParticleTable::GetParticleTable()->FindParticle(trajectory->GetPDGEncoding())->GetPDGMass();
                G4double energy = std::sqrt(trajectory->GetInitialMomentum().mag2() + mass * mass);
                G4double v_x = trajectory->GetVertexPosition().x() / mm;
                G4double v_y = trajectory->GetVertexPosition().y() / mm;
                G4double v_z = trajectory->GetVertexPosition().z() / mm;
                AddParticle(trajectory->GetPDGEncoding(), trajectory->GetInitialMomentum().x() / GeV, trajectory->GetInitialMomentum().y() / GeV, trajectory->GetInitialMomentum().z() / GeV,  energy /GeV, trajectory->GetParentID(), trajectory->GetTrackID(), v_x, v_y, v_z);
            }
        }else{
            G4cout << "No trajectory container found." << G4endl;
        }

        fRunAction->TransferData(EvtID, EvtID_Data);
        fRunAction->TransferData(ParticleEnergy, ParticleEnergy_Data);
        fRunAction->TransferData(ftagNulabel, ftagNulabel_Data);
        fRunAction->TransferData(interaction_x, Interaction_x_Data);
        fRunAction->TransferData(interaction_y, Interaction_y_Data);
        fRunAction->TransferData(interaction_z, Interaction_z_Data);
        fRunAction->TransferData(CaloEdepSum, CaloEdepSum_Data);
        fRunAction->TransferData(CaloVisibleEdepSum, CaloVisibleEdepSum_Data);
        fRunAction->TransferData(EcalEdepSum, EcalEdepSum_Data);
        fRunAction->TransferData(EcalVisibleEdepSum, EcalVisibleEdepSum_Data);
        fRunAction->TransferData(HcalEdepSum, HcalEdepSum_Data);
        fRunAction->TransferData(HcalVisibleEdepSum, HcalVisibleEdepSum_Data);
        fRunAction->TransferData(EcalMaxEdepCell, EcalMaxEdepCell_Data);
        fRunAction->TransferData(HcalMaxEdepCell, HcalMaxEdepCell_Data);
        fRunAction->TransferData(vecEcalCellID, vecEcalCellID_Data);
        fRunAction->TransferData(vecEcalStepsCell, vecEcalStepsCell_Data);
        fRunAction->TransferData(vecEcalEdepCell, vecEcalEdepCell_Data);
        fRunAction->TransferData(vecEcalVisibleEdepCell, vecEcalVisibleEdepCell_Data);
        fRunAction->TransferData(vecEcalHitTimeCell, vecEcalHitTimeCell_Data);
        fRunAction->TransferData(vecEcalToaCell, vecEcalToaCell_Data);
        fRunAction->TransferData(vecHcalCellID, vecHcalCellID_Data);
        fRunAction->TransferData(vecHcalStepsCell, vecHcalStepsCell_Data);
        fRunAction->TransferData(vecHcalEdepCell, vecHcalEdepCell_Data);
        fRunAction->TransferData(vecHcalVisibleEdepCell, vecHcalVisibleEdepCell_Data);
        fRunAction->TransferData(vecHcalHitTimeCell, vecHcalHitTimeCell_Data);
        fRunAction->TransferData(vecHcalToaCell, vecHcalToaCell_Data);
        fRunAction->TransferData(nstoredTruthParticles, nstoredTruthParticles_Data);
        fRunAction->TransferData(vecTruth_pdgID, vecTruth_pdgID_Data);
        fRunAction->TransferData(vecTruth_px, vecTruth_px_Data);
        fRunAction->TransferData(vecTruth_py, vecTruth_py_Data);
        fRunAction->TransferData(vecTruth_pz, vecTruth_pz_Data);
        fRunAction->TransferData(vecTruth_x, vecTruth_x_Data);
        fRunAction->TransferData(vecTruth_y, vecTruth_y_Data);
        fRunAction->TransferData(vecTruth_z, vecTruth_z_Data);
        fRunAction->TransferData(vecTruth_energy, vecTruth_energy_Data);
        fRunAction->TransferData(vecTruth_vertexIndex, vecTruth_vertexIndex_Data);
        fRunAction->TransferData(vecTruth_trackID, vecTruth_trackID_Data);
        fRunAction->FillEvent();
    }



    void EventAction::ResetEventData()
    {
        EvtID = 0;
        ParticleEnergy = 0;
        ftagNulabel = 0;
        interaction_x = 0;
        interaction_y = 0;
        interaction_z = 0;
        CaloEdepSum = 0;
        CaloVisibleEdepSum = 0;
        EcalEdepSum = 0;
        EcalVisibleEdepSum = 0;
        HcalEdepSum = 0;
        HcalVisibleEdepSum = 0;
        EcalMaxEdepCell = 0;
        HcalMaxEdepCell = 0;
        vecEcalCellID.clear();
        vecEcalStepsCell.clear();
        vecEcalEdepCell.clear();
        vecEcalVisibleEdepCell.clear();
        vecEcalHitTimeCell.clear();
        vecEcalToaCell.clear();
        vecHcalCellID.clear();
        vecHcalStepsCell.clear();
        vecHcalEdepCell.clear();
        vecHcalVisibleEdepCell.clear();
        vecHcalHitTimeCell.clear();
        vecHcalToaCell.clear();
        nstoredTruthParticles = 0;
        vecTruth_pdgID.clear();
        vecTruth_px.clear();
        vecTruth_py.clear();
        vecTruth_pz.clear();
        vecTruth_x.clear();
        vecTruth_y.clear();
        vecTruth_z.clear();
        vecTruth_energy.clear();
        vecTruth_vertexIndex.clear();
        vecTruth_trackID.clear();
    }

    void EventAction::AddParticle(int pdgID, double px, double py, double pz, double energy, int vertexIndex, int trackID, G4double v_x, G4double v_y, G4double v_z)
    {
        vecTruth_pdgID.push_back(pdgID);
        vecTruth_px.push_back(px);
        vecTruth_py.push_back(py);
        vecTruth_pz.push_back(pz);
        vecTruth_x.push_back(v_x);
        vecTruth_y.push_back(v_y);
        vecTruth_z.push_back(v_z);
        vecTruth_energy.push_back(energy);
        vecTruth_vertexIndex.push_back(vertexIndex);
        vecTruth_trackID.push_back(trackID);
        nstoredTruthParticles++;
    }
}
